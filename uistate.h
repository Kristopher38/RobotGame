#ifndef UISTATE_H
#define UISTATE_H

#include <memory>
#include "state.h"
#include "robotgame.h"
#include "pge/olcPixelGameEngine.h"
#include "pge/imgui_impl_pge.h"
#include "pge/imgui_stdlib.h"
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
        Block* target;
        olc::vi2d mousepos;

        IOSelectState(Block* target, olc::vi2d mousepos) : target(target), mousepos(mousepos) {}
        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
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

    class LinkingState : public IState
    {
    public:
        Block* target;
        IPort* port;

        LinkingState(Block* target, IPort* port) : target(target), port(port) {}

        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
        virtual void Update(RobotGame* game);
    };

    class IOSelectSecondState : public IState
    {
    public:
        Block* source;
        IPort* sourcePort;
        Block* target;
        olc::vi2d mousepos;

        IOSelectSecondState(Block* source, IPort* sourcePort, Block* target, olc::vi2d mousepos) : source(source), sourcePort(sourcePort), target(target), mousepos(mousepos) {}

        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
        virtual void Update(RobotGame* game) {}
    };
}

#endif // UISTATE_H
