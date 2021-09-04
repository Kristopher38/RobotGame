#include "programmableblock.h"

ProgrammableBlock::ProgrammableBlock(SpriteManager* sm, olc::vi2d pos) : Block(sm, pos, {1, 1}), running(false)
{
    this->toYield = this->warmupTime;
    this->SetSprite("code_block");
}

ProgrammableBlock::ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, int inputPorts, int outputPorts) : Block(sm, pos, {1, 1}, inputPorts, outputPorts), running(false)
{
    this->toYield = this->warmupTime;
    this->SetSprite("code_block");
}

ProgrammableBlock::ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts) : Block(sm, pos, {1, 1}, inputPorts, outputPorts), running(false)
{
    this->toYield = this->warmupTime;
    this->SetSprite("code_block");
}

ProgrammableBlock::ProgrammableBlock(const ProgrammableBlock& other) : Block(other)
{
    this->toYield = this->warmupTime;
    this->code = other.code;
    this->running = false;
}

ProgrammableBlock::~ProgrammableBlock()
{
    if (this->L)
        lua_close(this->L);
}

bool ProgrammableBlock::IsProgrammable()
{
    return true;
}

std::string ProgrammableBlock::GetDescription()
{
    return "Programmable block. Can be programmed in Lua to receive and respond to inputs "
           "from other components (e.g. buttons) and produce outputs which can be sent for "
           "further processing to another programmable blocks or connect to other components "
           "to control them.";
}

olc::Sprite* ProgrammableBlock::GetDefaultSprite()
{
    return this->sm->GetSprite("code_block").get();
}

ProgrammableBlock* ProgrammableBlock::Clone()
{
    return new ProgrammableBlock(*this);
}

static int luaPrint(lua_State* L)
{
    const char* str = luaL_checkstring(L, 1);
    std::cout<<str<<std::endl;
    ImGui::Begin("Debug");
    ImGui::Text("Lua:\n%s", str);
    ImGui::End();
    return 0;
}

static void luaHook(lua_State *L, lua_Debug *ar)
{
    ProgrammableBlock* pb = *(ProgrammableBlock**)lua_getextraspace(L);
    if (!pb->UpdateYieldTimer())
    {
        pb->SetRunning(false);
        luaL_error(L, "too long without yielding");
    }
}

void ProgrammableBlock::InitLua()
{
    this->L = luaL_newstate();

    // load libraries
    luaL_requiref(this->L, "basic", luaopen_base, false);
    // turn off dofile and loadfile to prevent any filesystem access
    lua_pushnil(this->L);
    lua_pushnil(this->L);
    lua_setglobal(this->L, "dofile");
    lua_setglobal(this->L, "loadfile");

    luaL_requiref(this->L, "coroutine", luaopen_coroutine, true);
    luaL_requiref(this->L, "string", luaopen_string, true);
    luaL_requiref(this->L, "utf8", luaopen_utf8, true);
    luaL_requiref(this->L, "table", luaopen_table, true);
    luaL_requiref(this->L, "math", luaopen_math, true);

    // setup custom functions
    lua_pushcfunction(this->L, luaPrint);
    lua_setglobal(this->L, "print");

    // setup 'ports' global
    this->SetupLuaPorts();

    // setup hook function
    *(ProgrammableBlock**)lua_getextraspace(this->L) = this;
    lua_sethook(this->L, luaHook, LUA_MASKCOUNT, this->hookInstrCount);
}

void ProgrammableBlock::SetupLuaPorts()
{
    lua_newtable(this->L);
    for (auto port : ports)
    {
        // t[portname] = portvalue
        lua_pushstring(this->L, port.first.c_str());
        this->PushDataValue(port.second->GetData());
        lua_settable(this->L, -3);
    }
    lua_setglobal(this->L, "ports");
}

std::string ProgrammableBlock::PopString()
{
    const char* str = luaL_checkstring(L, -1);
    if (str)
        return std::string(str);
    return std::string();
}

bool ProgrammableBlock::RunSetup()
{
    if (luaL_dostring(this->L, this->code.c_str()) == LUA_OK)
        return true;
    else
    {
        this->lastError = this->PopString();
        return false;
    }
}

bool ProgrammableBlock::RunUpdate()
{
    if (lua_getglobal(this->L, "update") != LUA_TFUNCTION)
    {
        lua_pop(this->L, 1);
        this->lastError = "Missing 'update' method";
        return false;
    }
    if (lua_pcall(this->L, 0, 0, 0) == LUA_OK)
        return true;
    else
    {
        this->lastError = this->PopString();
        return false;
    }
}

bool ProgrammableBlock::VerifyCode()
{
    lua_State* L = luaL_newstate();
    int res = luaL_loadstring(L, this->code.c_str());
    if (res == LUA_OK)
    {
        lua_close(L);
        return true;
    }
    else
    {
        const char* str = luaL_checkstring(L, -1);
        if (str)
            this->lastError = std::string(str);
        lua_close(L);
        return false;
    }
}

std::string ProgrammableBlock::GetError()
{
    return this->lastError;
}

void ProgrammableBlock::Start()
{
    this->InitLua();
    if (!this->VerifyCode())
        return;
    this->running = true;
    this->prevTime = std::chrono::steady_clock::now();
    this->toYield = this->warmupTime;
    this->RunSetup();
    this->toYield = this->yieldTime;
}

void ProgrammableBlock::Stop()
{
    this->running = false;
    lua_close(this->L);
}

void ProgrammableBlock::SetRunning(bool val)
{
    this->running = val;
}

void ProgrammableBlock::PushDataValue(DataValue data)
{
    if (std::holds_alternative<nil>(data))
        lua_pushnil(this->L);
    else if (std::holds_alternative<bool>(data))
        lua_pushboolean(this->L, std::get<bool>(data));
    else if (std::holds_alternative<int64_t>(data))
        lua_pushinteger(this->L, std::get<int64_t>(data));
    else if (std::holds_alternative<double>(data))
        lua_pushnumber(this->L, std::get<double>(data));
    else if (std::holds_alternative<std::string>(data))
        lua_pushstring(this->L, std::get<std::string>(data).c_str());
    else
        assert(false);
}

DataValue ProgrammableBlock::GetDataValue(int index)
{
    DataValue data;
    size_t len;

    int type = lua_type(this->L, index);
    switch (type)
    {
        case LUA_TBOOLEAN:
            data = lua_toboolean(this->L, -1);
            break;
        case LUA_TNUMBER:
            if (lua_isinteger(this->L, -1))
                data = lua_tointeger(this->L, -1);
            else
                data = lua_tonumber(this->L, -1);
            break;
        case LUA_TSTRING:
            data = std::string(lua_tolstring(this->L, -1, &len), len);
            break;
        case LUA_TNIL:
        default:
            data = nil();
            break;
    }
    return data;
}

bool ProgrammableBlock::UpdateYieldTimer()
{
    auto now = std::chrono::steady_clock::now();
    this->toYield -= now - this->prevTime;
    this->prevTime = now;

    if (this->toYield <= std::chrono::duration<int64_t, std::nano>::zero())
    {
        this->toYield = this->yieldTime;
        return false;
    }
    return true;
}

void ProgrammableBlock::DisplayError()
{
    ImGui::SetNextWindowSizeConstraints({200, 60}, {300, 150});
    ImGui::SetNextWindowPos({this->pos.x * 64, this->pos.y * 64});
    ImGui::Begin(("Error##" + std::to_string((intptr_t)this)).c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Tooltip);
    ImGui::TextWrapped(this->GetError().c_str());
    ImGui::End();
}

void ProgrammableBlock::Update(float timedelta)
{
    Block::Update(timedelta);

    if (this->running)
    {
        if (!this->RunUpdate())
            this->DisplayError();
        else
        {
            if (lua_getglobal(this->L, "ports") != LUA_TTABLE)
            {
                lua_pop(this->L, 1);
                this->SetupLuaPorts();
            }

            for (auto port : ports)
            {
                lua_pushstring(this->L, port.first.c_str());

                if (port.second->IsInput())
                {
                    auto data = port.second->GetData();
                    this->PushDataValue(data);
                    lua_settable(this->L, -3);
                }
                else if (port.second->IsOutput())
                {
                    lua_gettable(this->L, -2);
                    port.second->Update(this->GetDataValue(-1));
                    lua_pop(this->L, 1);
                }
            }
            lua_pop(this->L, 1);
        }
    }
    else
        this->DisplayError();
}
