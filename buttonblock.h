#ifndef BUTTONBLOCK_H
#define BUTTONBLOCK_H

#include "block.h"

class RobotGame;

class ButtonBlock : public Block
{
public:
    ButtonBlock(olc::vi2d pos, std::shared_ptr<olc::Sprite> sprite) : Block(pos, {1, 1}, sprite, {}, {"btn_value"}) {}
    ButtonBlock(const ButtonBlock& other) : Block(other) {}
    virtual std::string GetDescription();
    virtual ButtonBlock* Clone();
    virtual void HandleInput(RobotGame* game);
};

#endif // BUTTONBLOCK_H
