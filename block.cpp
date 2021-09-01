#include "block.h"

Block::Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite, int inputPorts, int outputPorts) : Block(pos, size, sprite)
{
    for (int i = 0; i < inputPorts; ++i)
        this->ports["Input " + std::to_string(i)] = std::make_shared<InputPort>(this);
    for (int i = 0; i < inputPorts; ++i)
        this->ports["Output " + std::to_string(i)] = std::make_shared<OutputPort>(this);
}

Block::Block(olc::vi2d pos, olc::vi2d size, std::shared_ptr<olc::Sprite> sprite, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts) : pos(pos), size(size), img(sprite)
{
    for (auto port : inputPorts)
        this->ports[port] = std::make_shared<InputPort>(this);
    for (auto port : outputPorts)
        this->ports[port] = std::make_shared<OutputPort>(this);
}

Block::Block(const Block& other)
{
    this->img = other.img;
    this->size = other.size;
    this->pos = other.pos;
    for (auto port : other.ports)
    {
        auto newPort = std::shared_ptr<IPort>(port.second->Clone());
        newPort->owner = this;
        this->ports[port.first] = newPort;
    }
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
