#include "motorblock.h"

MotorBlock::MotorBlock(const MotorBlock& other) : Block(other)
{
    this->robot = other.robot;
    this->motorState = {{SIDE::UP, false}, {SIDE::DOWN, false}, {SIDE::LEFT, false}, {SIDE::RIGHT, false}};
}

std::string MotorBlock::GetDescription()
{
    return "Motor controller. Allows you to move the robot around. "
           "4 inputs correspond to 4 sides the robot can move to. "
           "Each input moves the robot one tile in the specific direction on the rising "
           "edge of the input signal. Motor controller only accepts true/false values. "
           "Rising edge means change from false to true. "
           "For repeated movement you have to supply "
           "multiple rising edges as an input over multiple update() calls.";
}

std::shared_ptr<olc::Sprite> MotorBlock::GetDefaultSprite()
{
    return this->sm->GetSprite("motor");
}

MotorBlock* MotorBlock::Clone()
{
    return new MotorBlock(*this);
}

void MotorBlock::Update(float timedelta)
{
    static const std::map<std::string, SIDE> portSides = {{"up", SIDE::UP}, {"down", SIDE::DOWN}, {"left", SIDE::LEFT}, {"right", SIDE::RIGHT}};
    for (const auto& [name, side] : portSides)
    {
        DataValueEx portVal = this->ports[name]->GetData();
        if (std::holds_alternative<bool>(portVal))
        {
            bool newVal = std::get<bool>(portVal);
            if (newVal && !this->motorState[side])
                this->robot->Move(side);
            this->motorState[side] = newVal;
        }
    }
}
