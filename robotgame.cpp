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
    this->gameLayer = this->CreateLayer();
    this->EnableLayer(this->gameLayer, true);

    sprites.push_back(std::make_shared<olc::Sprite>("assets/default_block.png"));
    sprites.push_back(std::make_shared<olc::Sprite>("assets/default_big_block.png"));
    sprites.push_back(std::make_shared<olc::Sprite>("assets/code_block.png"));
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            blocks.push_back(Block({i, j}, {1, 1}, sprites[0]));
    blocks.push_back(Block({6, 6}, {2, 2}, sprites[1]));
    blocks.push_back(ProgrammableBlock({1, 6}, sprites[2]));
    return true;
}

void RobotGame::DrawGrid()
{
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
    this->DrawPartialSprite(block->pos * blocksize,
                            block->GetSprite(),
                            {0, 0},
                            block->size * blocksize);
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

bool RobotGame::OnUserUpdate(float fElapsedTime)
{
    this->SetDrawTarget(this->gameLayer);
    this->Clear(olc::BLACK);
    this->DrawGrid();

    this->HandleInput();
    this->UpdateState();

    for (auto block = blocks.begin(); block != blocks.end(); ++block)
        this->DrawBlock(&(*block));

    ImGui::Begin("TestWindow");
    ImGui::InputTextMultiline("", &codebuf, {300, 500}, ImGuiInputTextFlags_AllowTabInput);
    ImGui::End();
    ImGui::ShowDemoWindow();

    return true;
}

void RobotGame::DrawUI(void)
{
    pge_imgui.ImGui_ImplPGE_Render();
}
