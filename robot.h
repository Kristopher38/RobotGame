#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <string>

#include "pge/olcPixelGameEngine.h"
#include "block.h"
#include "map.h"
#include "spritemanager.h"
#include "programmableblock.h"

class RobotGame;

enum class SIDE {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};

class Robot
{
private:
    Map* map;
    SpriteManager* sm;
    RobotGame* game;


    bool CanMove(olc::vi2d pos);
public:
    olc::vi2d pos;

    Robot(RobotGame* game, Map* map, SpriteManager* sm, olc::vi2d pos) : game(game), map(map), sm(sm), pos(pos) {}

    bool Move(SIDE side);
    bool PickUp();
    bool Drop();
    std::vector<std::string> RadarQuery();
};

#endif // ROBOT_H
