#ifndef UISTATE_H
#define UISTATE_H

#include <memory>
#include "state.h"
#include "robotgame.h"
#include "pge/olcPixelGameEngine.h"
#include "block.h"

class RobotGame;

namespace UIState
{
    class EditIdleState : public IState
    {
    public:
        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
        virtual void Update(RobotGame* game) {}
    };

    class IOSelectState : public IState
    {
    public:
        virtual std::unique_ptr<IState> HandleInput(RobotGame* game) {}
        virtual void Update(RobotGame* game) {}
    };

    class DraggingState : public IState
    {
    public:
        Block* target;
        olc::vi2d startpos;
        olc::vi2d handlepos;

        DraggingState(Block* target, olc::vi2d startpos, olc::vi2d handlepos) : target(target), startpos(startpos), handlepos(handlepos) {}

        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
        virtual void Update(RobotGame* game) {}
    };
}

#endif // UISTATE_H
