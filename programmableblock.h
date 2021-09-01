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
    ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite);
    ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite, int inputPorts, int outputPorts);
    ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts);
    ProgrammableBlock(const ProgrammableBlock& other);
    ~ProgrammableBlock();

    virtual bool IsProgrammable();
    virtual std::string GetDescription();
    virtual ProgrammableBlock* Clone();

    bool RunOnce();
    bool VerifyCode();
    std::string GetError();
};

#endif // PROGRAMMABLEBLOCK_H
