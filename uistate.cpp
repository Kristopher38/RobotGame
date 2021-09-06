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
        if (game->InvCount(invBlock) > 0)
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
    ImGui::SetNextWindowSize({(float)200, (float)(35 + (this->target->ports.size() * 20) + (this->target->IsProgrammable() ? 35 : 0))});
    ImGui::SetNextWindowPos({(float)this->mousepos.x, (float)this->mousepos.y});
    ImGui::Begin("IO", NULL, game->popUpMenuFlags);

    if (!ImGui::IsWindowFocused())
        return std::make_unique<UIState::EditIdleState>();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    auto ports = &this->target->ports;
    static int openNode = -1;
    int idx = 0;
    for (auto it = ports->begin(); it != ports->end(); ++it, ++idx)
    {
        bool open = ImGui::TreeNodeEx(std::to_string(idx).c_str(), flags, it->first.c_str());
        bool link = false;
        if (it->second->CanConnect())
        {
            ImGui::SameLine(0.0f, 15.0f);
            link = ImGui::SmallButton(("Link##" + std::to_string(idx)).c_str());
        }

        if (open)
            openNode = idx;

        if (open && idx == openNode)
        {
            auto conns = it->second->connections;
            for (auto conn = conns.begin(); conn != conns.end();)
            {
                if ((*conn)->owner)
                {
                    ImGui::BulletText((*conn)->owner->GetPortName(*conn).c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton(("X##" + std::to_string(idx)).c_str()))
                        it->second->Disconnect(*conn);
                    else
                        ++conn;
                }
            }

            ImGui::TreePop();
        }
        if (link)
            return std::make_unique<UIState::LinkingState>(this->target, it->second.get());
    }

    if (this->target->IsProgrammable() && ImGui::Button("Edit code", {80, 30}))
        return std::make_unique<UIState::CodeEditState>(dynamic_cast<ProgrammableBlock*>(this->target));

    if (ImGui::Button("Remove", {80, 30}))
    {
        game->IncrInvCount(this->target->schema, 1);
        game->RemoveBlock(this->target);
        return std::make_unique<UIState::EditIdleState>();
    }

    ImGui::End();
    return nullptr;
}

void UIState::IOSelectState::OnExit(RobotGame* game)
{
    ImGui::End();
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
    ImGui::SetNextWindowSize({120.0, (float)(this->target->ports.size() * 20)});
    ImGui::SetNextWindowPos({(float)this->mousepos.x, (float)this->mousepos.y});
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
        if (this->sourcePort->CanConnect(it->second.get()))
        {
            if (ImGui::Selectable((it->first + "##" + std::to_string(idx++)).c_str()))
            {
                this->sourcePort->Connect(it->second.get());
                ImGui::End();
                return std::make_unique<UIState::EditIdleState>();
            }
        }
        else
            ImGui::Selectable((it->first.c_str()), false, ImGuiSelectableFlags_Disabled);
    }

    ImGui::End();

    return nullptr;
}

std::unique_ptr<IState> UIState::CodeEditState::HandleInput(RobotGame* game)
{
    static bool open = true;
    ImGui::SetNextWindowSizeConstraints({300, 200}, {800, 600});
    if (open)
    {
        if (ImGui::Begin("Code Editor", &open, game->codeEditFlags))
        {
            ImVec2 textInputSize = ImGui::GetContentRegionAvail();
            textInputSize.y = -40;
            ImGui::InputTextMultiline("##code", &this->target->code, textInputSize, ImGuiInputTextFlags_AllowTabInput);
            if (!this->target->VerifyCode())
                ImGui::TextWrapped(this->target->GetError().c_str());
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

        if (game->InvCount(this->target->schema) > 0)
        {
            game->PlaceBlock(this->target);
            game->IncrInvCount(this->target->schema, -1);
            return std::make_unique<UIState::EditIdleState>();
        }
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
   if (game->GetKey(olc::Key::M).bPressed)
   {
       game->EnableLayer(game->blocksUILayer, true);
       game->EnableLayer(game->connectionsLayer, true);
       return std::make_unique<UIState::EditIdleState>();
   }
   return nullptr;
}

void UIState::InteractIdleState::Update(RobotGame* game)
{
    float tpsInv = 1.0f / game->tps;
    this->timeCounter += game->timedelta;
    if (this->timeCounter >= tpsInv)
    {
        this->timeCounter = this->timeCounter - tpsInv;
        game->SimTick();
    }
}

void UIState::InteractIdleState::OnEnter(RobotGame* game)
{
    this->timeCounter = 0.0f;
    game->SimStart();
}

void UIState::InteractIdleState::OnExit(RobotGame* game)
{
    game->SimStop();
}
