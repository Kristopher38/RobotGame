#ifndef MOTORBLOCK_H
#define MOTORBLOCK_H

#include "block.h"
#include "robot.h"

class MotorBlock : public Block
{
private:
	std::map<SIDE, bool> motorState;
	Robot* robot;
public:
	MotorBlock(SpriteManager* sm, Robot* robot, olc::vi2d pos) : Block(sm, pos, {2, 2}, {"up", "down", "left", "right"}, {}), robot(robot), motorState{{SIDE::UP, false}, {SIDE::DOWN, false}, {SIDE::LEFT, false}, {SIDE::RIGHT, false}}
	{
		this->SetSprite(this->GetDefaultSprite());
	}
	MotorBlock(const MotorBlock& other);

	virtual std::string GetDescription();
	virtual std::shared_ptr<olc::Sprite> GetDefaultSprite();
	virtual MotorBlock* Clone();

	virtual void Update(float timedelta);
};

#endif // MOTORBLOCK_H
