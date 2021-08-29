#ifndef PROGRAMMABLEBLOCK_H
#define PROGRAMMABLEBLOCK_H

#include <string>
#include <memory>
#include "pge/olcPixelGameEngine.h"
#include "block.h"

class ProgrammableBlock : public Block
{
public:
	std::string code;
    ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite) : Block(pos, {1, 1}, sprite) {}
    ProgrammableBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite, std::string code) : Block(pos, {1, 1}, sprite), code(code) {}
};

#endif // PROGRAMMABLEBLOCK_H
