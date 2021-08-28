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
	int x;
	int y;
	int w;
	int h;

    Block() : x(0), y(0), w(1), h(1) {}
    Block(int x, int y) : x(x), y(y), w(1), h(1) {}
	Block(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    Block(int x, int y, int w, int h, std::shared_ptr<olc::Sprite> sprite) : x(x), y(y), w(w), h(h), img(sprite) {}

    virtual void SetSprite(std::shared_ptr<olc::Sprite> sprite);
    olc::Sprite* GetSprite();
};

#endif // BLOCK_H
