#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define OLC_PGE_APPLICATION

#include "robotgame.h"
#include <iostream>
RobotGame::RobotGame() : pge_imgui(true)
{
    this->sAppName = "Robot game";
    this->state = std::make_unique<UIState::EditIdleState>();
}

bool RobotGame::OnUserCreate()
{
    this->tempLayer = this->CreateLayer();
    this->connectionsLayer = this->CreateLayer();
    this->blocksUILayer = this->CreateLayer();
    this->infoUILayer = this->CreateLayer();
    this->mapGridLayer = this->CreateLayer();
    this->gridLayer = this->CreateLayer();

    this->EnableLayer(this->tempLayer, true);
    this->EnableLayer(this->connectionsLayer, true);
    this->EnableLayer(this->blocksUILayer, true);
    this->EnableLayer(this->infoUILayer, true);
    this->EnableLayer(this->mapGridLayer, true);
    this->EnableLayer(this->gridLayer, true);

    this->ClearLayers();

    this->sm.LoadAll();

    inventory.push_back(std::make_shared<ProgrammableBlock>(&sm, olc::vi2d{0, 0}, std::vector<std::string>{"input_1"}, std::vector<std::string>{"output_1"}));
    inventory.push_back(std::make_shared<ButtonBlock>(&sm, olc::vi2d{0, 0}));
    inventory.push_back(std::make_shared<DiodeBlock>(&sm, olc::vi2d{0, 0}));
    return true;
}

void RobotGame::DrawGrid()
{
    this->SetDrawTarget(this->gridLayer);
    for (int i = 1; i <= gridSize.x; i++)
    {
        this->DrawLine({i * blocksize - 1, 0}, {i * blocksize - 1, this->gridSize.y * blocksize - 1}, olc::GREY);
        for (int j = 1; j <= this->gridSize.y; j++)
            this->DrawLine({0, j * blocksize - 1}, {this->gridSize.x * blocksize - 1, j * blocksize - 1}, olc::GREY);
    }
}

bool RobotGame::CursorCollide(olc::vi2d pos, olc::vi2d size, olc::vi2d mousepos)
{
    return pos.x <= mousepos.x &&
           pos.x + size.x > mousepos.x &&
           pos.y <= mousepos.y &&
           pos.y + size.y > mousepos.y;
}

Block* RobotGame::GetBlockAt(olc::vi2d pos)
{
    for (auto block = blocks.begin(); block != blocks.end(); ++block)
    {
        if (this->CursorCollide(block->get()->pos * blocksize, block->get()->size * blocksize, pos))
            return block->get();
    }
    return nullptr;
}

Block* RobotGame::GetBlockUnderMouse()
{
    return this->GetBlockAt(this->GetMousePos());
}

olc::vi2d RobotGame::GetGridAt(olc::vi2d pos)
{
    return {pos.x / blocksize, pos.y / blocksize};
}

bool RobotGame::CanBePlaced(Block* block, olc::vi2d gridpos)
{
    for (int i = 0; i < block->size.x; ++i)
        for (int j = 0; j < block->size.y; ++j)
        {
            olc::vi2d curGridPos = gridpos + olc::vi2d(i, j);
            Block* testedBlock = this->GetBlockAt(curGridPos * blocksize);
            if (testedBlock && testedBlock != block || curGridPos.x >= this->gridSize.x || curGridPos.y >= this->gridSize.y)
                return false;
        }
    return true;
}

void RobotGame::DrawBlock(Block* block)
{
    this->DrawBlockSprite(block->pos * blocksize, block);
}

void RobotGame::DrawBlockSprite(olc::vi2d pos, Block* block)
{
    this->DrawSprite(pos, block->GetSprite(), spritescale);
}

void RobotGame::SelectBlock(Block* block)
{
    this->selectedBlock = block;
}

void RobotGame::HandleInput()
{
    if (this->IsFocused())
    {
        InputState input;
        input.kbd = this->pKeyboardState;
        input.mousepos = this->GetMousePos();
        input.lmb = this->GetMouse(olc::Mouse::LEFT);
        input.rmb = this->GetMouse(olc::Mouse::RIGHT);
        input.mmb = this->GetMouse(olc::Mouse::MIDDLE);

        std::unique_ptr<IState> newState = this->state->HandleInput(this);
        if (newState)
        {
            this->state->OnExit(this);
            this->state = std::move(newState);
            this->state->OnEnter(this);
        }

        for (auto block : blocks)
        {
            block->HandleInput(this->GetBlockUnderMouse() == block.get(), &input);
        }
    }
}

void RobotGame::UpdateState()
{
    this->state.get()->Update(this);
}

void RobotGame::DrawInfoUI()
{
    this->infoMenuPos.x = gridSize.x * blocksize;
    this->SetDrawTarget(this->infoUILayer);
    this->Clear(olc::BLANK);

    if (this->selectedBlock)
    {
        olc::Sprite* sprite = this->selectedBlock->GetSprite();
        this->DrawBlockSprite(this->infoMenuPos + olc::vi2d{uiPadding, 0}, this->selectedBlock);

        olc::vi2d pos = this->infoMenuPos;
        pos.y += sprite->height * spritescale;
        olc::vi2d size = this->GetWindowSize() - pos;
        ImGui::SetNextWindowPos({pos.x, pos.y});
        ImGui::SetNextWindowSize({size.x, size.y});
        ImGui::Begin("##Info", NULL, this->infoMenuFlags);
        ImGui::TextWrapped(this->selectedBlock->GetDescription().c_str());
        ImGui::End();
    }
}

olc::vi2d RobotGame::GetBlockCenter(Block* block)
{
    return ((block->pos * 2 + block->size) * blocksize) / 2;
}

void RobotGame::DrawConnections()
{
    this->SetDrawTarget(this->connectionsLayer);
    for (auto block = blocks.begin(); block != blocks.end(); ++block)
    {
        for (auto port = block->get()->ports.begin(); port != block->get()->ports.end(); ++port)
        {
            for (auto conn = port->second->connections.begin(); conn != port->second->connections.end(); ++conn)
            {
                this->DrawLine(this->GetBlockCenter(block->get()), this->GetBlockCenter((*conn)->owner), olc::GREEN);
            }
        }
    }
}

void RobotGame::DrawBlocks()
{
    this->SetDrawTarget(this->mapGridLayer);
    for (auto block = blocks.begin(); block != blocks.end(); ++block)
        this->DrawBlock(block->get());
}

void RobotGame::ClearLayers()
{
    this->SetDrawTarget(this->connectionsLayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->gridLayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->infoUILayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->blocksUILayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->tempLayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->mapGridLayer);
    this->Clear(olc::BLANK);
}

void RobotGame::ShowDebugWindow()
{
    static bool gridEnabled = true;
    static bool connEnabled = true;
    static bool infoUIEnabled = true;
    static bool blocksUIEnabled = true;

    ImGui::Begin("Debug");

    ImGui::Checkbox("gridLayer", &gridEnabled);
    ImGui::Checkbox("connectionsLayer", &connEnabled);
    ImGui::Checkbox("infoMenuLayer", &infoUIEnabled);
    ImGui::Checkbox("blocksMenuLayer", &blocksUIEnabled);

    ImGui::SliderInt("GridSize X", &gridSize.x, 1, 20);
    ImGui::SliderInt("GridSize Y", &gridSize.y, 1, 20);

    Block* block = this->GetBlockUnderMouseInv();
    if (block)
        ImGui::Text("%s", block->GetDescription().c_str());

    ImGui::End();

//    this->EnableLayer(gridLayer, gridEnabled);
//    this->EnableLayer(connectionsLayer, connEnabled);
//    this->EnableLayer(infoUILayer, infoUIEnabled);
//    this->EnableLayer(blocksUILayer, blocksUIEnabled);
}

olc::vi2d RobotGame::GetResolution()
{
    int w = this->gridSize.x * this->blocksize + this->infoMenuSize.x;
    int h = std::max(this->gridSize.y * this->blocksize, this->infoMenuSize.y);

    return {w, h};
}

void RobotGame::PlaceBlock(std::shared_ptr<Block> block)
{
    this->blocks.push_back(block);
}

Block* RobotGame::GetBlockUnderMouseInv()
{
    int posx = 0;
    for (auto block : inventory)
    {
        if (this->CursorCollide({posx, this->blocksMenuPos.y}, block->size * blocksize, this->GetMousePos()))
            return block.get();
        posx += block->size.x * blocksize + uiPadding;
    }
    return nullptr;
}

void RobotGame::DrawBlocksUI()
{
    this->SetDrawTarget(this->blocksUILayer);
    this->blocksMenuPos = {0, this->gridSize.y * blocksize};

    int posx = 0;
    for (auto block : inventory)
    {
        this->DrawBlockSprite({posx, this->blocksMenuPos.y}, block.get());
        posx += block->size.x * blocksize + uiPadding;
    }
}

void RobotGame::SimStart()
{
    for (auto block : blocks)
        block->Start();
}

void RobotGame::SimStop()
{
    for (auto block : blocks)
        block->Stop();
}

void RobotGame::SimTick()
{
    for (auto block : blocks)
        block->Update(this->timedelta);
    for (auto block : blocks)
        block->Swap();
}

bool RobotGame::OnUserUpdate(float fElapsedTime)
{
    this->timedelta = fElapsedTime;
    this->ClearLayers();

    this->HandleInput();
    this->UpdateState();

    this->DrawGrid();
    this->DrawBlocksUI();
    this->DrawInfoUI();
    this->DrawConnections();
    this->DrawBlocks();

    ImGui::ShowDemoWindow();
    this->ShowDebugWindow();

    return true;
}

void RobotGame::DrawUI(void)
{
    pge_imgui.ImGui_ImplPGE_Render();
}
