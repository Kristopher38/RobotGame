#ifndef MAPBLOCK_H
#define MAPBLOCK_H

#include "block.h"
#include "pge/olcPixelGameEngine.h"

class MapBlock : public Block
{
private:
	std::shared_ptr<olc::Sprite> pallete;
public:
	MapBlock(SpriteManager* sm, olc::vi2d pos);
	MapBlock(const MapBlock& other) : Block(other) { this->pallete = other.pallete; }

	virtual std::string GetDescription();
	virtual std::shared_ptr<olc::Sprite> GetDefaultSprite() { return this->sm->GetSprite("screen"); }
	virtual MapBlock* Clone() { return new MapBlock(*this); }

	virtual void Start();
	virtual void Update(float timedelta);
	virtual void Stop();
};

#endif // MAPBLOCK_H
