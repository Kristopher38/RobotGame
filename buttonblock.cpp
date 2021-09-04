#include "buttonblock.h"
#include "robotgame.h"

ButtonBlock::ButtonBlock(const ButtonBlock& other) : Block(other)
{
    this->state = other.state;
}

std::string ButtonBlock::GetDescription()
{
    return "Button block. Returns 0 or 1 on its output depending on whether it's being pressed or not";
}

std::shared_ptr<olc::Sprite> ButtonBlock::GetDefaultSprite()
{
    return this->sm->GetSprite("button");
}

ButtonBlock* ButtonBlock::Clone()
{
    return new ButtonBlock(*this);
}

void ButtonBlock::HandleInput(bool isPointedAt, InputState* input)
{
    if (isPointedAt)
        this->state = input->lmb.bHeld;
    else
        this->state = false;
}

void ButtonBlock::Update(float timedelta)
{
    Block::Update(timedelta);
    if (this->state)
    {
        this->SetSprite("button_pressed");
        this->ports["btn_value"]->Update(true);
    }
    else
    {
        this->SetSprite("button");
        this->ports["btn_value"]->Update(false);
    }
}

void ButtonBlock::Stop()
{
    this->state = false;
    this->SetSprite("button");
}
