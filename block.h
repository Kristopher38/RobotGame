#ifndef BLOCK_H
#define BLOCK_H

#include <memory>
#include "pge/olcPixelGameEngine.h"

class Block
{
private:
    std::shared_ptr<olc::Sprite> img;
    // ??? outputs;
    // ??? inputs;
public:
    olc::vi2d pos;
    olc::vi2d size;

    Block() : pos(0, 0), size(1, 1) {}
    Block(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite) : pos(pos), size(1, 1), img(sprite) {}
    Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite) : pos(pos), size(size), img(sprite) {}

    virtual void SetSprite(std::shared_ptr<olc::Sprite> sprite);
    olc::Sprite* GetSprite();
};

#endif // BLOCK_H
