#include "diodeblock.h"
#include "robotgame.h"

DiodeBlock::DiodeBlock(const DiodeBlock& other) : Block(other)
{
    this->state = other.state;
}

std::string DiodeBlock::GetDescription()
{
    return "Diode block. Any non-zero value at its input turns it on, zero value turns it off.";
}

DiodeBlock* DiodeBlock::Clone()
{
    return new DiodeBlock(*this);
}

void DiodeBlock::HandleInput(bool isPointedAt, InputState* input)
{}

void DiodeBlock::Update(float timedelta)
{
    Block::Update(timedelta);
    auto s = this->ports["diode_val"]->GetData();

    if (std::holds_alternative<bool>(s))
        this->state = std::get<bool>(s);
    else if (std::holds_alternative<int64_t>(s))
        this->state = std::get<int64_t>(s) != 0;
    else if (std::holds_alternative<double>(s))
        this->state = std::get<double>(s) != 0.0;

    if (this->state)
        this->SetSprite("diode_on");
    else
        this->SetSprite("diode_off");
}

void DiodeBlock::Stop()
{
    this->state = false;
    this->SetSprite("diode_off");
}
