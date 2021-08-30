#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define OLC_PGE_APPLICATION

#include "robotgame.h"

RobotGame::RobotGame() : pge_imgui(true)
{
    this->sAppName = "Robot game";
    this->state = std::make_unique<UIState::EditIdleState>();
}

bool RobotGame::OnUserCreate()
{
    this->connectionsLayer = this->CreateLayer();
    this->gridLayer = this->CreateLayer();
    this->blocksUILayer = this->CreateLayer();
    this->infoUILayer = this->CreateLayer();
    this->EnableLayer(this->gridLayer, true);
    this->EnableLayer(this->blocksUILayer, false);
    this->EnableLayer(this->infoUILayer, true);
    this->EnableLayer(this->connectionsLayer, true);
    this->ClearLayers();
    //this->SetLayerOffset(this->infoUILayer, {80, 0});

    sprites.push_back(std::make_shared<olc::Sprite>("assets/default_block.png"));
    sprites.push_back(std::make_shared<olc::Sprite>("assets/default_big_block.png"));
    sprites.push_back(std::make_shared<olc::Sprite>("assets/code_block.png"));
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
        {
            blocks.push_back(Block({i, j}, {1, 1}, sprites[0]));
            blocks.back().ports.push_back(std::make_shared<InputPort>(&blocks.back()));
            blocks.back().ports.push_back(std::make_shared<InputPort>(&blocks.back()));
            blocks.back().ports.push_back(std::make_shared<OutputPort>(&blocks.back()));
            blocks.back().ports.push_back(std::make_shared<OutputPort>(&blocks.back()));
        }
    blocks.push_back(Block({6, 6}, {2, 2}, sprites[1]));
    blocks.back().description = "Big block";
    blocks.push_back(ProgrammableBlock({1, 6}, sprites[2]));
    blocks.back().description = "Programmable block";
    return true;
}

void RobotGame::DrawGrid()
{
    this->SetDrawTarget(this->gridLayer);
    this->Clear(olc::BLANK);
    for (int i = 1; i <= gridSize.x; i++)
    {
        this->DrawLine({i * blocksize - 1, 0}, {i * blocksize - 1, this->gridSize.y * blocksize - 1}, olc::GREY);
        for (int j = 1; j <= this->gridSize.y; j++)
            this->DrawLine({0, j * blocksize - 1}, {this->gridSize.x * blocksize - 1, j * blocksize - 1}, olc::GREY);
    }
}

Block* RobotGame::GetBlockAt(olc::vi2d pos)
{
    for (auto block = blocks.begin(); block != blocks.end(); ++block)
    {
        Block bScreen = *block;
        bScreen.pos *= blocksize;
        bScreen.size *= blocksize;
        if (bScreen.pos.x <= pos.x &&
            bScreen.pos.x + bScreen.size.x > pos.x &&
            bScreen.pos.y <= pos.y &&
            bScreen.pos.y + bScreen.size.y > pos.y)
            return &(*block);
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
    this->DrawBlockSprite(block->pos * blocksize, block->GetSprite());
//    this->DrawPartialSprite(block->pos * blocksize,
//                            block->GetSprite(),
//                            {0, 0},
//                            block->size * (blocksize / spritescale),
//                            spritescale);
}

void RobotGame::DrawBlockSprite(olc::vi2d pos, olc::Sprite* sprite)
{
    this->DrawSprite(pos, sprite, spritescale);
}

void RobotGame::SelectBlock(Block* block)
{
    this->selectedBlock = block;
}

void RobotGame::HandleInput()
{
    if (this->IsFocused())
    {
        std::unique_ptr<IState> newState = this->state->HandleInput(this);
        if (newState)
            state = std::move(newState);
    }
}

void RobotGame::UpdateState()
{
    this->state.get()->Update(this);
}

void RobotGame::DrawInfoUI()
{
    this->SetDrawTarget(this->infoUILayer);
    if (this->selectedBlock)
    {
        olc::Sprite* sprite = this->selectedBlock->GetSprite();
        this->DrawBlockSprite(this->infoUIPos, sprite);
        this->DrawString(this->infoUIPos + olc::vi2d{0, sprite->height * spritescale + 4}, this->selectedBlock->GetDescription());
    }
}

olc::vi2d RobotGame::GetBlockCenter(Block* block)
{
    return block->pos * blocksize + olc::vi2d{blocksize/2, blocksize/2};
}

void RobotGame::DrawConnections()
{
    this->SetDrawTarget(this->connectionsLayer);
    for (auto block = blocks.begin(); block != blocks.end(); ++block)
    {
        for (auto port = block->ports.begin(); port != block->ports.end(); ++port)
        {
            for (auto conn = port->get()->connections.begin(); conn != port->get()->connections.end(); ++conn)
            {
                this->DrawLine(this->GetBlockCenter(&(*block)), this->GetBlockCenter((*conn)->owner), olc::GREEN);
            }
        }
    }
}

void RobotGame::ClearLayers()
{
    this->SetDrawTarget(this->connectionsLayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->gridLayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->blocksUILayer);
    this->Clear(olc::BLANK);
    this->SetDrawTarget(this->infoUILayer);
    this->Clear(olc::BLANK);
}

bool RobotGame::OnUserUpdate(float fElapsedTime)
{
    this->ClearLayers();

    this->HandleInput();
    this->UpdateState();

    this->DrawGrid();
    //this->DrawBlocksUI();
    this->DrawInfoUI();
    this->DrawConnections();

    this->SetDrawTarget(this->gridLayer);
    for (auto block = blocks.begin(); block != blocks.end(); ++block)
        this->DrawBlock(&(*block));

    ImGui::ShowDemoWindow();

    return true;
}

void RobotGame::DrawUI(void)
{
    pge_imgui.ImGui_ImplPGE_Render();
}
