#include "uistate.h"

std::unique_ptr<IState> UIState::EditIdleState::HandleInput(RobotGame* game)
{
    olc::HWButton lmb = game->GetMouse(olc::Mouse::LEFT);
    olc::HWButton rmb = game->GetMouse(olc::Mouse::RIGHT);
    olc::vi2d pos = game->GetMousePos();
    Block* block = game->GetBlockAt(pos);
    if (lmb.bPressed && block)
    {
        game->SelectBlock(block);
        olc::vi2d gridpos = game->GetGridAt(pos);
        return std::make_unique<UIState::DraggingState>(block, gridpos, gridpos - block->pos);
    }
    else if (rmb.bPressed && block)
    {
        //game->ShowIOSelection(block);
        return std::make_unique<UIState::IOSelectState>(block, pos);
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
    return nullptr;
}

std::unique_ptr<IState> UIState::IOSelectState::HandleInput(RobotGame* game)
{
    const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |
                                         ImGuiWindowFlags_NoScrollbar |
                                         ImGuiWindowFlags_NoMove |
                                         ImGuiWindowFlags_NoResize |
                                         ImGuiWindowFlags_NoCollapse |
                                         ImGuiWindowFlags_NoNav |
                                         ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowSize({80, 200});
    ImGui::SetNextWindowPos({this->mousepos.x, this->mousepos.y});
    ImGui::Begin("IO", NULL, windowFlags);

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
        int idx = it->get()->GetType() == PortType::INPUT ? ++output : ++input;
        if (ImGui::Selectable((it->get()->GetTypeStr() + std::to_string(idx)).c_str()))
        {
            ImGui::End();
            return std::make_unique<UIState::LinkingState>(this->target, it->get());
        }
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
    game->SetDrawTarget(game->connectionsLayer);
    game->DrawLine(this->target->pos * game->blocksize + olc::vi2d{game->blocksize/2, game->blocksize/2}, game->GetMousePos(), olc::GREEN);
}

std::unique_ptr<IState> UIState::IOSelectSecondState::HandleInput(RobotGame* game)
{
    const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |
                                         ImGuiWindowFlags_NoScrollbar |
                                         ImGuiWindowFlags_NoMove |
                                         ImGuiWindowFlags_NoResize |
                                         ImGuiWindowFlags_NoCollapse |
                                         ImGuiWindowFlags_NoNav |
                                         ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowSize({80, 200});
    ImGui::SetNextWindowPos({this->mousepos.x, this->mousepos.y});
    ImGui::Begin("IO", NULL, windowFlags);

    if (!ImGui::IsWindowFocused())
    {
        ImGui::End();
        return std::make_unique<UIState::EditIdleState>();
    }

    int idx = 0;
    auto ports = &this->target->ports;
    for (auto it = ports->begin(); it != ports->end(); ++it)
    {
        if (it->get()->GetType() != this->sourcePort->GetType())
        {
            if (ImGui::Selectable((it->get()->GetTypeStr() + std::to_string(++idx)).c_str()))
            {
                this->sourcePort->Connect(it->get());
                ImGui::End();
                return std::make_unique<UIState::EditIdleState>();
            }
        }
        else
            ImGui::Selectable((it->get()->GetTypeStr() + std::to_string(++idx)).c_str(), false, ImGuiSelectableFlags_Disabled);
    }

    ImGui::End();

    return nullptr;
}
