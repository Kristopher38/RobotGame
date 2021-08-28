#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define OLC_PGE_APPLICATION

#include "robotgame.h"

RobotGame::RobotGame() : pge_imgui(true)
{
    sAppName = "Robot game";
}

bool RobotGame::OnUserCreate()
{
    this->gameLayer = this->CreateLayer();
    this->EnableLayer(this->gameLayer, true);

    sprites.push_back(std::make_shared<olc::Sprite>("assets/default_block.png"));
    sprites.push_back(std::make_shared<olc::Sprite>("assets/default_big_block.png"));
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            blocks.push_back(Block(i, j, 1, 1, sprites[0]));
    blocks.push_back(Block(6, 6, 2, 2, sprites[1]));
    return true;
}

void RobotGame::DrawGrid(int blocksize)
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
        if (block->x * blocksize <= pos.x &&
            block->x * blocksize + block->w * blocksize > pos.x &&
            block->y * blocksize <= pos.y &&
            block->y * blocksize + block->h * blocksize > pos.y)
            return &(*block);
    }
    return nullptr;
}

olc::vi2d RobotGame::GetGridAt(olc::vi2d pos)
{
    return {pos.x / blocksize, pos.y / blocksize};
}

bool RobotGame::CanBePlaced(Block* block, olc::vi2d pos)
{
    olc::vi2d gridpos = this->GetGridAt(pos);
    for (int i = 0; i < block->w; ++i)
        for (int j = 0; j < block->h; ++j)
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
    this->DrawPartialSprite({block->x * blocksize, block->y * blocksize},
                            block->GetSprite(),
                            {0, 0},
                            {block->w * blocksize, block->h * blocksize});
}

bool RobotGame::OnUserUpdate(float fElapsedTime)
{
    this->SetDrawTarget(this->gameLayer);
    this->Clear(olc::BLACK);
    this->DrawGrid(10);

    if (this->IsFocused())
    {
        if (this->GetMouse(0).bHeld)
        {
            Block* block = this->GetBlockAt(this->GetMousePos());
            if (block && this->state != STATE::DRAGGING)
            {
                this->state = STATE::DRAGGING;
                this->dragging = block;
                //this->DrawString({0, 90}, "Block found");
            }
            else if (this->state == STATE::DRAGGING)
            {
                olc::vi2d pos = this->GetMousePos();
                olc::vi2d gridpos = this->GetGridAt(pos);
                if (this->CanBePlaced(this->dragging, pos))
                {
                    this->dragging->x = gridpos.x;
                    this->dragging->y = gridpos.y;
                }
            }
        }
        else if (this->state == STATE::DRAGGING)
        {
            this->state = STATE::IDLE;
            this->dragging = nullptr;
        }
        this->DrawString({0, 90}, state == STATE::IDLE ? "Idle" : "Dragging");
    }

    for (auto block = blocks.begin(); block != blocks.end(); ++block)
        this->DrawBlock(&(*block));

    ImGui::ShowDemoWindow();

    return true;
}

void RobotGame::DrawUI(void)
{
    pge_imgui.ImGui_ImplPGE_Render();
}
