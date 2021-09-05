#include "robot.h"

bool Robot::CanMove(olc::vi2d pos)
{
    return this->map->GetPos("walls", pos) == 0;
}

bool Robot::Move(SIDE side)
{
    switch (side)
    {
    case SIDE::UP:
        if (this->CanMove(this->pos + olc::vi2d{0, -1}))
            this->pos.y--;
            return true;
    case SIDE::DOWN:
        if (this->CanMove(this->pos + olc::vi2d{0, 1}))
            this->pos.y++;
            return true;
    case SIDE::LEFT:
        if (this->CanMove(this->pos + olc::vi2d{-1, 0}))
            this->pos.x--;
            return true;
    case SIDE::RIGHT:
        if (this->CanMove(this->pos + olc::vi2d{1, 0}))
            this->pos.x++;
            return true;
    }
}

bool Robot::PickUp()
{
    int tile = this->map->GetPos("floor", this->pos);

    switch (tile)
    {
        case 1:
        this->inventory = std::make_shared<ProgrammableBlock>(sm, olc::vi2d{0, 0}, std::vector<std::string>{"input_1"}, std::vector<std::string>{"output_1"});
        this->map->SetPos("floor", this->pos, 0);
    }
}
