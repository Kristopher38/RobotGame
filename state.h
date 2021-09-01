#ifndef STATE_H
#define STATE_H

#include <memory>

class RobotGame;

class IState
{
public:
	virtual ~IState() {}
	virtual std::unique_ptr<IState> HandleInput(RobotGame* game) = 0;
    virtual void Update(RobotGame* game) {}
    virtual void OnEnter(RobotGame* game) {}
    virtual void OnExit(RobotGame* game) {}
};

#endif // STATE_H
