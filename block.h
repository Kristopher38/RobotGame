#ifndef BLOCK_H
#define BLOCK_H

#include <memory>
#include <string>
#include "pge/olcPixelGameEngine.h"
#include "port.h"

class Block
{
private:
    std::shared_ptr<olc::Sprite> img;

public:
    std::vector<std::shared_ptr<IPort>> ports;
    std::string description;
    olc::vi2d pos;
    olc::vi2d size;

    Block() : pos(0, 0), size(1, 1) {}
    Block(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite) : pos(pos), size(1, 1), img(sprite) {}
    Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite) : pos(pos), size(size), img(sprite) {}

    virtual void SetSprite(std::shared_ptr<olc::Sprite> sprite);
    olc::Sprite* GetSprite();
    std::string GetDescription();
};

#endif // BLOCK_H
