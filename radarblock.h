#ifndef RADARBLOCK_H
#define RADARBLOCK_H

#include "block.h"
#include "map.h"
#include "robot.h"

class RadarBlock : public Block
{
private:
	Map* map;
	Robot* robot;
public:
	RadarBlock(SpriteManager* sm, Map* map, Robot* robot, olc::vi2d pos) : Block(sm, pos, {2, 2}, {"position_input"}, {"data_output"}), map(map), robot(robot) { this->SetSprite("radar"); }
	RadarBlock(const RadarBlock& other) : Block(other)
	{
		this->map = other.map;
		this->robot = other.robot;
	}

	virtual std::string GetDescription();
	virtual std::shared_ptr<olc::Sprite> GetDefaultSprite() { return this->sm->GetSprite("radar"); }
	virtual RadarBlock* Clone() { return new RadarBlock(*this); }

	virtual void Update(float timedelta);
};

#endif // RADARBLOCK_H
