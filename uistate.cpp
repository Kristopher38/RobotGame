#include "uistate.h"

std::unique_ptr<IState> UIState::EditIdleState::HandleInput(RobotGame* game)
{
    olc::HWButton lmb = game->GetMouse(olc::Mouse::LEFT);
    olc::HWButton rmb = game->GetMouse(olc::Mouse::RIGHT);
    olc::vi2d pos = game->GetMousePos();
    Block* block = game->GetBlockAt(pos);
    Block* invBlock = game->GetBlockUnderMouseInv();
    if (lmb.bPressed && block)
    {
        game->SelectBlock(block);
        olc::vi2d gridpos = game->GetGridAt(pos);
        return std::make_unique<UIState::DraggingState>(block, gridpos, gridpos - block->pos);
    }
    else if (rmb.bPressed && block)
    {
        return std::make_unique<UIState::IOSelectState>(block, pos);
    }
    else if (lmb.bPressed && invBlock)
    {
        return std::make_unique<UIState::BlockPlaceState>(std::shared_ptr<Block>(invBlock->Clone()));
    }
    else if (game->GetKey(olc::Key::M).bPressed)
    {
        game->EnableLayer(game->blocksUILayer, false);
        game->EnableLayer(game->connectionsLayer, false);
        return std::make_unique<UIState::InteractIdleState>();
    }
    return nullptr;
}

std::unique_ptr<IState> UIState::DraggingState::HandleInput(RobotGame* game)
{
    olc::HWButton lmb = game->GetMouse(olc::Mouse::LEFT);
    if (lmb.bHeld)
    {
        ImGui::Begin("Debug");
        ImGui::Text("%d %d", this->handlepos.x, this->handlepos.y);
        ImGui::End();
        olc::vi2d pos = game->GetMousePos();
        olc::vi2d gridpos = game->GetGridAt(pos);
        if (game->CanBePlaced(this->target, gridpos - this->handlepos))
            this->target->pos = gridpos - this->handlepos;
        return nullptr;
    }
    else if (lmb.bReleased)
        return std::make_unique<UIState::EditIdleState>();
    return nullptr;
}

std::unique_ptr<IState> UIState::IOSelectState::HandleInput(RobotGame* game)
{
    ImGui::SetNextWindowSize({120, 200});
    ImGui::SetNextWindowPos({this->mousepos.x, this->mousepos.y});
    ImGui::Begin("IO", NULL, game->popUpMenuFlags);

    if (!ImGui::IsWindowFocused())
    {
        ImGui::End();
        return std::make_unique<UIState::EditIdleState>();
    }

    int output = 0;
    int input = 0;
    auto ports = &this->target->ports;
    for (auto it = ports->begin(); it != ports->end(); ++it)
    {
        int idx = it->second->GetType() == PortType::INPUT ? ++output : ++input;
        if (ImGui::Selectable((it->first + "##" + std::to_string(idx++)).c_str()))
        {
            ImGui::End();
            return std::make_unique<UIState::LinkingState>(this->target, it->second.get());
        }
    }
    if (this->target->IsProgrammable() && ImGui::Button("Edit code", {80, 30}))
    {
        ImGui::End();
        return std::make_unique<UIState::CodeEditState>(dynamic_cast<ProgrammableBlock*>(this->target));
    }

    ImGui::End();

    return nullptr;
}

std::unique_ptr<IState> UIState::LinkingState::HandleInput(RobotGame* game)
{
    Block* block = game->GetBlockUnderMouse();
    if (game->GetMouse(olc::Mouse::LEFT).bPressed && block)
        return std::make_unique<UIState::IOSelectSecondState>(this->target, this->port, block, game->GetMousePos());
    else if (game->GetMouse(olc::Mouse::RIGHT).bPressed)
        return std::make_unique<UIState::EditIdleState>();

    return nullptr;
}

void UIState::LinkingState::Update(RobotGame* game)
{
    game->SetDrawTarget(game->tempLayer);
    game->DrawLine(game->GetBlockCenter(this->target), game->GetMousePos(), olc::GREEN);
}

std::unique_ptr<IState> UIState::IOSelectSecondState::HandleInput(RobotGame* game)
{
    ImGui::SetNextWindowSize({120, 200});
    ImGui::SetNextWindowPos({this->mousepos.x, this->mousepos.y});
    ImGui::Begin("IO", NULL, game->popUpMenuFlags);

    if (!ImGui::IsWindowFocused())
    {
        ImGui::End();
        return std::make_unique<UIState::EditIdleState>();
    }

    int idx = 0;
    auto ports = &this->target->ports;
    for (auto it = ports->begin(); it != ports->end(); ++it)
    {
        if (it->second->GetType() != this->sourcePort->GetType())
        {
            if (ImGui::Selectable((it->first + "##" + std::to_string(idx++)).c_str()))
            {
                this->sourcePort->Connect(it->second.get());
                ImGui::End();
                return std::make_unique<UIState::EditIdleState>();
            }
        }
        else
            ImGui::Selectable((it->second->GetTypeStr() + std::to_string(++idx)).c_str(), false, ImGuiSelectableFlags_Disabled);
    }

    ImGui::End();

    return nullptr;
}

std::unique_ptr<IState> UIState::CodeEditState::HandleInput(RobotGame* game)
{
    static ImVec2 size{300, 200};
    static ImVec2 pos{32, 32};
    static bool open = true;
    //ImGui::SetNextWindowSize({300, 200});
    //ImGui::SetNextWindowPos({0, 0});
    if (open)
    {
        if (ImGui::Begin("Code Editor", &open, game->codeEditFlags))
        {
            ImGui::InputTextMultiline("##code", &this->target->code, {300, 400}, ImGuiInputTextFlags_AllowTabInput);
            if (!this->target->VerifyCode())
                ImGui::Text(this->target->GetError().c_str());
        }
        ImGui::End();
        return nullptr;
    }
    else
    {
        open = true;    // restore original state for next time the window is opened
        return std::make_unique<UIState::EditIdleState>();
    }
    return nullptr;
}

std::unique_ptr<IState> UIState::BlockPlaceState::HandleInput(RobotGame* game)
{
    if (game->GetMouse(olc::Mouse::RIGHT).bPressed)
        return std::make_unique<UIState::EditIdleState>();
    else if (game->GetMouse(olc::Mouse::LEFT).bPressed && game->CanBePlaced(this->target.get(), this->target->pos))
    {
        this->target->pos = game->GetGridAt(game->GetMousePos());
        game->PlaceBlock(this->target);
        return std::make_unique<UIState::EditIdleState>();
    }
    return nullptr;
}

void UIState::BlockPlaceState::Update(RobotGame* game)
{
    game->SetDrawTarget(game->tempLayer);
    this->target->pos = game->GetGridAt(game->GetMousePos());
    if (game->CanBePlaced(this->target.get(), this->target->pos))
        game->DrawBlock(this->target.get());
}

std::unique_ptr<IState> UIState::InteractIdleState::HandleInput(RobotGame* game)
{
   Block* block = game->GetBlockUnderMouse();
   if (game->GetKey(olc::Key::M).bPressed)
   {
       game->EnableLayer(game->blocksUILayer, true);
       game->EnableLayer(game->connectionsLayer, true);
       return std::make_unique<UIState::EditIdleState>();
   }
   else if (block)
   {
       block->HandleInput(game);
   }
   return nullptr;
}

void UIState::InteractIdleState::Update(RobotGame* game)
{
    game->SimTick();
}
