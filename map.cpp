#include "map.h"

Map::Map()
{
    this->L = luaL_newstate();
    luaL_openlibs(this->L);
}

Map::~Map()
{
    lua_close(this->L);
}

int Map::GetPos(std::string layer, olc::vi2d pos)
{
    lua_getglobal(this->L, "GetPos");
    lua_pushstring(this->L, layer.c_str());
    lua_pushinteger(this->L, pos.x);
    lua_pushinteger(this->L, pos.y);
    lua_pcall(this->L, 3, 1, 0);
    int res = lua_tonumber(this->L, -1);
    lua_pop(this->L, 1);
    return res;
}

void Map::SetPos(std::string layer, olc::vi2d pos, int val)
{
    lua_getglobal(this->L, "SetPos");
    lua_pushstring(this->L, layer.c_str());
    lua_pushinteger(this->L, pos.x);
    lua_pushinteger(this->L, pos.y);
    lua_pushinteger(this->L, val);
    lua_pcall(this->L, 4, 0, 0);
}

void Map::Load()
{
    if (luaL_loadfile(this->L, this->mapFilePath.string().c_str()) != LUA_OK)
        throw std::runtime_error("Couldn't load map manager script, ensure assets/scripts/mapmanager.lua exists");
    if (lua_pcall(this->L, 0, LUA_MULTRET, 0) != LUA_OK)
        throw std::runtime_error("Couldn't run map manager script: " + std::string(lua_tostring(this->L, -1)));
}
