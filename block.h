#ifndef BLOCK_H
#define BLOCK_H

#include <memory>
#include <string>
#include "pge/olcPixelGameEngine.h"
#include "port.h"
#include "spritemanager.h"
#include "inputstate.h"

class RobotGame;

class Block
{
protected:
    std::shared_ptr<olc::Sprite> img;
    SpriteManager* sm;

public:
    std::map<std::string, std::shared_ptr<IPort>> ports;
    olc::vi2d pos;
    olc::vi2d size;
    const Block* schema;

    Block() : pos(0, 0), size(1, 1) {}
    Block(SpriteManager* sm, olc::vi2d pos);
    Block(SpriteManager* sm, olc::vi2d pos, olc::vi2d size);
    Block(SpriteManager* sm, olc::vi2d pos, olc::vi2d size, int inputPorts, int outputPorts);
    Block(SpriteManager* sm, olc::vi2d pos, olc::vi2d size, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts);
    Block(const Block& other);

    void SetSprite(std::string sprite);
    void SetSprite(std::shared_ptr<olc::Sprite> sprite);
    olc::Sprite* GetSprite();
    std::string GetPortName(IPort* port);

    virtual std::string GetDescription();
    virtual std::shared_ptr<olc::Sprite> GetDefaultSprite();
    virtual bool IsProgrammable();
    virtual Block* Clone();
    virtual void HandleInput(bool isPointedAt, InputState* input) {}
    virtual void Update(float timedelta);
    virtual void Swap();
    virtual void Start() {}
    virtual void Stop() {}
};

#endif // BLOCK_H
