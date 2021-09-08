#include "robot.h"
#include "robotgame.h"

bool Robot::CanMove(olc::vi2d pos)
{
    return this->map->GetPos("walls", pos) == 0;
}

bool Robot::Move(SIDE side)
{
    bool moved = false;
    switch (side)
    {
    case SIDE::UP:
        if (this->CanMove(this->pos + olc::vi2d{0, -1}))
        {
            this->pos.y--;
            moved = true;
        }
        break;
    case SIDE::DOWN:
        if (this->CanMove(this->pos + olc::vi2d{0, 1}))
        {
            this->pos.y++;
            moved = true;
        }
        break;
    case SIDE::LEFT:
        if (this->CanMove(this->pos + olc::vi2d{-1, 0}))
        {
            this->pos.x--;
            moved = true;
        }
        break;
    case SIDE::RIGHT:
        if (this->CanMove(this->pos + olc::vi2d{1, 0}))
        {
            this->pos.x++;
            moved = true;
        }
        break;
    }
    if (this->map->GetPos("items", this->pos))
        this->PickUp();
    return moved;
}

bool Robot::PickUp()
{
    int item = this->map->GetPos("items", this->pos);

    switch (item)
    {
        case 1:
            game->inventory[1].first++;
            break;
        case 3:
            game->inventory[0].first++;
            break;
        case 6:
            game->inventory[2].first++;
            break;
        case 10:
            game->inventory[5].first++;
            break;
        default:
            return false;
    }
    this->map->SetPos("items", this->pos, 0);
    return true;
}
