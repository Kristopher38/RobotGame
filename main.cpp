#include <iostream>
#include "robotgame.h"

using namespace std;

int main()
{
    RobotGame demo;
    if (demo.Construct(256, 144, 4, 4, false, true))
        demo.Start();
    return 0;
}
