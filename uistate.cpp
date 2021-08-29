#include "uistate.h"

std::unique_ptr<IState> UIState::EditIdleState::HandleInput(RobotGame* game)
{
    olc::HWButton lmb = game->GetMouse(olc::Mouse::LEFT);
    olc::HWButton rmb = game->GetMouse(olc::Mouse::RIGHT);
    olc::vi2d pos = game->GetMousePos();
    Block* block = game->GetBlockAt(pos);
    if (lmb.bPressed && block)
    {
        //game->DisplayBlockInfo(block);
        olc::vi2d gridpos = game->GetGridAt(pos);
        return std::make_unique<UIState::DraggingState>(block, gridpos, gridpos - block->pos);
    }
    else if (rmb.bPressed && block)
    {
        //game->ShowIOSelection(block);
        return std::make_unique<UIState::IOSelectState>();
    }
    return nullptr;
}

std::unique_ptr<IState> UIState::DraggingState::HandleInput(RobotGame* game)
{
    olc::HWButton lmb = game->GetMouse(olc::Mouse::LEFT);
    if (lmb.bHeld)
    {
        olc::vi2d pos = game->GetMousePos();
        olc::vi2d gridpos = game->GetGridAt(pos);
        if (game->CanBePlaced(this->target, gridpos - this->handlepos))
            this->target->pos = gridpos - this->handlepos;
        return nullptr;
    }
    else if (lmb.bReleased)
        return std::make_unique<UIState::EditIdleState>();
}
