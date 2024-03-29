#include <iostream>
#include "robotgame.h"
#include "lua/lua.hpp"

#include <variant>

using namespace std;

int main()
{
    RobotGame demo;
    olc::vi2d res = demo.GetResolution();
    if (demo.Construct(res.x, res.y, 1, 1, false, true))
        demo.Start();

    return 0;
}
