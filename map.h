#ifndef MAP_H
#define MAP_H

#include <filesystem>
#include "lua/lua.hpp"
#include <type_traits>
#include "pge/olcPixelGameEngine.h"

class Map
{
private:
    const std::filesystem::path mapFilePath = {"assets/scripts/mapmanager.lua"};
    lua_State* L;

public:
	Map();
    ~Map();

    int GetPos(std::string layer, olc::vi2d pos);
    void SetPos(std::string layer, olc::vi2d pos, int val);

    template<typename T> T GetLayerProperty(std::string layer, std::string property)
    {
        lua_getglobal(this->L, "GetLayerProperty");
        lua_pushstring(this->L, layer.c_str());
        lua_pushstring(this->L, property.c_str());
        lua_pcall(this->L, 2, 1, 0);

        T val;
        if constexpr (std::is_same_v<T, std::string>)
        {
            val = T(lua_tostring(this->L, -1));
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            val = lua_toboolean(this->L, -1);
        }
        else if constexpr (std::is_integral<T>::value)
        {
            val = lua_tointeger(this->L, -1);
        }
        else if constexpr (std::is_floating_point<T>::value)
        {
            val = lua_tonumber(this->L, -1);
        }
        lua_pop(this->L, 1);
        return val;
    }
	void Load();
};

#endif // MAP_H
