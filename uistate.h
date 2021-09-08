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
    };

    class IOSelectState : public IState
    {
    public:
        Block* target;
        ImVec2 mousepos;

        IOSelectState(Block* target, ImVec2 mousepos) : target(target), mousepos(mousepos) {}
        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
        void OnExit(RobotGame* game);
    };

    class DraggingState : public IState
    {
    public:
        Block* target;
        olc::vi2d startpos;
        olc::vi2d handlepos;

        DraggingState(Block* target, olc::vi2d startpos, olc::vi2d handlepos) : target(target), startpos(startpos), handlepos(handlepos) {}

        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
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
        ImVec2 mousepos;

        IOSelectSecondState(Block* source, IPort* sourcePort, Block* target, ImVec2 mousepos) : source(source), sourcePort(sourcePort), target(target), mousepos(mousepos) {}

        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
    };

    class CodeEditState : public IState
    {
    public:
        ProgrammableBlock* target;

        CodeEditState(ProgrammableBlock* target) : target(target) {}

        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
    };

    class BlockPlaceState : public IState
    {
    public:
        std::shared_ptr<Block> target;

        BlockPlaceState(std::shared_ptr<Block> target) : target(target) {}

        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
        virtual void Update(RobotGame* game);
    };

    class InteractIdleState : public IState
    {
    private:
        float timeCounter;
    public:
        virtual std::unique_ptr<IState> HandleInput(RobotGame* game);
        virtual void Update(RobotGame* game);
        void OnEnter(RobotGame* game);
        void OnExit(RobotGame* game);
    };
}

#endif // UISTATE_H
