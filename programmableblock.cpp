#include "programmableblock.h"

ProgrammableBlock::ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite) : Block(pos, {1, 1}, sprite)
{
    this->L = luaL_newstate();
}

ProgrammableBlock::ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite, int inputPorts, int outputPorts) : Block(pos, {1, 1}, sprite, inputPorts, outputPorts)
{
    this->L = luaL_newstate();
}

ProgrammableBlock::ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts) : Block(pos, {1, 1}, sprite, inputPorts, outputPorts)
{
    this->L = luaL_newstate();
}

ProgrammableBlock::ProgrammableBlock(const ProgrammableBlock& other) : Block(other)
{
    this->code = other.code;
    this->L = luaL_newstate();
}

ProgrammableBlock::~ProgrammableBlock()
{
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

ProgrammableBlock* ProgrammableBlock::Clone()
{
    return new ProgrammableBlock(*this);
}

bool ProgrammableBlock::RunOnce()
{
    int res;
    if (this->VerifyCode())
    {
        if (luaL_dostring(this->L, this->code.c_str()) == LUA_OK)
        {
            return true;
        }
    }
    return false;
}

bool ProgrammableBlock::VerifyCode()
{
    int res = luaL_loadstring(this->L, this->code.c_str());
    if (res == LUA_OK)
    {
        lua_pop(this->L, 1);
        return true;
    }
    else
        return false;
}

std::string ProgrammableBlock::GetError()
{
    const char* err = luaL_checkstring(this->L, -1);
    std::string errstr = std::string(err);
    lua_pop(this->L, 1);
    return errstr;
}
