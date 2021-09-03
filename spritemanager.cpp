#include "spritemanager.h"

bool SpriteManager::instantiated = false;

SpriteManager::SpriteManager()
{
    assert(!this->instantiated);
    this->instantiated = true;
}
SpriteManager::~SpriteManager()
{
    this->instantiated = false;
}

void SpriteManager::LoadAll()
{
    for (auto const& file : std::filesystem::directory_iterator(this->assets))
    {
        std::filesystem::path fpath = file.path();
        if (fpath.extension().string() == std::string(".png"))
            this->sprites[fpath.stem().string()] = std::make_shared<olc::Sprite>(fpath.string());
    }
}

void SpriteManager::Load(std::string name)
{

}

std::shared_ptr<olc::Sprite> SpriteManager::GetSprite(std::string name)
{
    if (this->sprites.count(name))
        return this->sprites[name];
    else
    {
        this->Load(name);
        return this->sprites[name];
    }
}
