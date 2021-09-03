#ifndef DIODEBLOCK_H
#define DIODEBLOCK_H

#include "block.h"

class DiodeBlock : public Block
{
private:
	bool state;
public:
	DiodeBlock(SpriteManager* sm, olc::vi2d pos) : Block(sm, pos, {1, 1}, {"diode_val"}, {}), state(false) { this->SetSprite("diode_off"); }
	DiodeBlock(const DiodeBlock& other);

    virtual std::string GetDescription();
    virtual olc::Sprite* GetDefaultSprite();
    virtual DiodeBlock* Clone();

    virtual void HandleInput(bool isPointedAt, InputState* input);
    virtual void Update(float timedelta);
    virtual void Stop();
};

#endif // DIODEBLOCK_H
