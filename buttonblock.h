#ifndef BUTTONBLOCK_H
#define BUTTONBLOCK_H

#include "block.h"

class RobotGame;

class ButtonBlock : public Block
{
private:
    bool state;
public:
    ButtonBlock(SpriteManager* sm, olc::vi2d pos) : Block(sm, pos, {1, 1}, {}, {"btn_value"}), state(false) { this->SetSprite("button"); }
    ButtonBlock(const ButtonBlock& other);
    virtual std::string GetDescription();
    virtual olc::Sprite* GetDefaultSprite();
    virtual ButtonBlock* Clone();
    virtual void HandleInput(bool isPointedAt, InputState* input);
    virtual void Update(float timedelta);
    virtual void Stop();
};

#endif // BUTTONBLOCK_H
