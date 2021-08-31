#include "block.h"

Block::Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite, int inputPorts, int outputPorts) : pos(pos), size(size), img(sprite)
{
    for (int i = 0; i < inputPorts; ++i)
        this->ports.push_back(std::make_shared<InputPort>(this, "Input " + std::to_string(i)));
    for (int i = 0; i < outputPorts; ++i)
        this->ports.push_back(std::make_shared<OutputPort>(this, "Output " + std::to_string(i)));
}

Block::Block(const Block& other)
{
    this->img = other.img;
    this->size = other.size;
    this->pos = other.pos;
    for (auto port : ports)
        this->ports.push_back(std::shared_ptr<IPort>(port->Clone()));
}

void Block::SetSprite(std::shared_ptr<olc::Sprite> sprite)
{
    this->img = sprite;
}

olc::Sprite* Block::GetSprite()
{
    return this->img.get();
}

std::string Block::GetDescription()
{
    return "Generic block";
}

bool Block::IsProgrammable()
{
    return false;
}

Block* Block::Clone()
{
    return new Block(*this);
}
