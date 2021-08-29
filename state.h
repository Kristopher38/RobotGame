#ifndef STATE_H
#define STATE_H

#include <memory>

class RobotGame;

class IState
{
public:
	virtual ~IState() {}
	virtual std::unique_ptr<IState> HandleInput(RobotGame* game) = 0;
	virtual void Update(RobotGame* game) = 0;
};

#endif // STATE_H
