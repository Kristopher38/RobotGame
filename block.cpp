#include "block.h"

Block::Block(SpriteManager* sm, olc::vi2d pos) : Block(sm, pos, {1, 1}) {}

Block::Block(SpriteManager* sm, olc::vi2d pos, olc::vi2d size) : sm(sm), pos(pos), size(size), schema(nullptr)
{
    this->SetSprite(this->GetDefaultSprite());
}

Block::Block(SpriteManager* sm, olc::vi2d pos, olc::vi2d size, int inputPorts, int outputPorts) : Block(sm, pos, size)
{
    for (int i = 0; i < inputPorts; ++i)
        this->ports["Input " + std::to_string(i)] = std::make_shared<InputPort>(this);
    for (int i = 0; i < inputPorts; ++i)
        this->ports["Output " + std::to_string(i)] = std::make_shared<OutputPort>(this);
}

Block::Block(SpriteManager* sm, olc::vi2d pos, olc::vi2d size, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts) : Block(sm, pos, size)
{
    for (auto port : inputPorts)
        this->ports[port] = std::make_shared<InputPort>(this);
    for (auto port : outputPorts)
        this->ports[port] = std::make_shared<OutputPort>(this);
}

Block::Block(const Block& other)
{
    this->sm = other.sm;
    this->img = other.img;
    this->size = other.size;
    this->pos = other.pos;
    this->schema = other.schema ? other.schema : &other;
    for (auto port : other.ports)
    {
        auto newPort = std::shared_ptr<IPort>(port.second->Clone());
        newPort->owner = this;
        this->ports[port.first] = newPort;
    }
}

void Block::SetSprite(std::string name)
{
    this->img = sm->GetSprite(name);
}

void Block::SetSprite(std::shared_ptr<olc::Sprite> sprite)
{
    this->img = sprite;
}

olc::Sprite* Block::GetSprite()
{
    return this->img.get();
}

std::shared_ptr<olc::Sprite> Block::GetDefaultSprite()
{
    return this->sm->GetSprite("default_block");
}

std::string Block::GetPortName(IPort* other)
{
    for (auto port : this->ports)
    {
        if (port.second.get() == other)
            return port.first;
    }
    return std::string();
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

void Block::Update(float timedelta) {}

void Block::Swap()
{
    for (auto port : ports)
        port.second->Propagate();
    for (auto port : ports)
        port.second->Swap();
}
