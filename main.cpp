#include <iostream>
#include "robotgame.h"

using namespace std;

int main()
{
    RobotGame demo;
    if (demo.Construct(300*4, 144*4, 1, 1, false, true))
        demo.Start();
    return 0;
}
