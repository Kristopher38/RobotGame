#ifndef PROGRAMMABLEBLOCK_H
#define PROGRAMMABLEBLOCK_H

#include <string>
#include <memory>
#include "pge/olcPixelGameEngine.h"
#include "lua/lua.hpp"
#include "block.h"

class ProgrammableBlock : public Block
{
public:
	std::string code;
    lua_State* L;
    ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite) : Block(pos, {1, 1}, sprite) {}
    ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite, int inputPorts, int outputPorts) : Block(pos, {1, 1}, sprite, inputPorts, outputPorts) {}

    ProgrammableBlock(const ProgrammableBlock& other);

    virtual bool IsProgrammable();
    virtual std::string GetDescription();
    virtual ProgrammableBlock* Clone();
};

#endif // PROGRAMMABLEBLOCK_H
