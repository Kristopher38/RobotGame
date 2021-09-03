#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <memory>
#include <string>
#include <cassert>
#include <filesystem>
#include "pge/olcPixelGameEngine.h"

class SpriteManager
{
private:
	static bool instantiated;
	const std::filesystem::path assets = {"assets/sprites"};
	std::map<std::string, std::shared_ptr<olc::Sprite>> sprites;
public:
	SpriteManager();
	~SpriteManager();

	void LoadAll();
	void Load(std::string name);
	std::shared_ptr<olc::Sprite> GetSprite(std::string name);
};

#endif // SPRITEMANAGER_H
