#include <iostream>
#include "robotgame.h"

using namespace std;

int main()
{
    RobotGame demo;
    if (demo.Construct(240, 120, 8, 8, false, true))
        demo.Start();
    return 0;
}