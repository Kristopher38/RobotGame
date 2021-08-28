#include "block.h"

void Block::SetSprite(std::shared_ptr<olc::Sprite> sprite)
{
    this->img = sprite;
}

olc::Sprite* Block::GetSprite()
{
    return this->img.get();
}
