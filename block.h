#ifndef BLOCK_H
#define BLOCK_H

#include <memory>
#include <string>
#include "pge/olcPixelGameEngine.h"
#include "port.h"

class RobotGame;

class Block
{
private:
    std::shared_ptr<olc::Sprite> img;

public:
    std::map<std::string, std::shared_ptr<IPort>> ports;
    olc::vi2d pos;
    olc::vi2d size;

    Block() : pos(0, 0), size(1, 1) {}
    Block(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite) : pos(pos), size(1, 1), img(sprite) {}
    Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite) : pos(pos), size(size), img(sprite) {}
    Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite, int inputPorts, int outputPorts);
    Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts);
    Block(const Block& other);

    void SetSprite(std::shared_ptr<olc::Sprite> sprite);
    olc::Sprite* GetSprite();

    virtual std::string GetDescription();
    virtual bool IsProgrammable();
    virtual Block* Clone();
    virtual void HandleInput(RobotGame* game) {}
};

#endif // BLOCK_H
