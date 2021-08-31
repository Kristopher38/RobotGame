#include <iostream>
#include "robotgame.h"

using namespace std;

int main()
{
    RobotGame demo;
    olc::vi2d res = demo.GetResolution();
    if (demo.Construct(res.x, res.y, 1, 1, false, true))
        demo.Start();

    return 0;
}
