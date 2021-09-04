#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <string>

#include "pge/olcPixelGameEngine.h"
#include "block.h"
#include "map.h"
#include "spritemanager.h"
#include "programmableblock.h"

enum class SIDE {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};

class Robot
{
private:
    Map* map;
    SpriteManager* sm;
    olc::vi2d pos;
    SIDE facing;
    std::shared_ptr<Block> inventory;

    bool CanMove(olc::vi2d pos);
public:
    Robot(Map* map, SpriteManager* sm, olc::vi2d pos, SIDE facing) : map(map), sm(sm), pos(pos), facing(facing) {}

    bool Move(SIDE side);
    bool Turn(SIDE side);
    bool PickUp();
    bool Drop();
    std::vector<std::string> RadarQuery();
};

#endif // ROBOT_H
