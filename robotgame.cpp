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
    this->connectionsLayer = this->CreateLayer();
    this->gridLayer = this->CreateLayer();
    this->blocksUILayer = this->CreateLayer();
    this->infoUILayer = this->CreateLayer();
    this->EnableLayer(this->gridLayer, true);
    this->EnableLayer(this->blocksUILayer, true);
    this->EnableLayer(this->infoUILayer, true);
    this->EnableLayer(this->connectionsLayer, true);
    this->ClearLayers();

    std::filesystem::path assets("assets/sprites");
    for (auto const& file : std::filesystem::directory_iterator(assets))
    {
        std::filesystem::path fpath = file.path();
        if (fpath.extension().string() == std::string(".png"))
            sprites[fpath.stem().string()] = std::make_shared<olc::Sprite>(fpath.string());
    }

    //sprites.push_back(std::make_shared<olc::Sprite>("assets/default_block.png"));
    //sprites.push_back(std::make_shared<olc::Sprite>("assets/default_big_block.png"));
    //sprites.push_back(std::make_shared<olc::Sprite>("assets/code_block.png"));
    inventory.push_back(std::make_shared<Block>(olc::vi2d{0, 0}, olc::vi2d{1, 1}, sprites["default_block"], 2, 2));
    inventory.push_back(std::make_shared<Block>(olc::vi2d{0, 0}, olc::vi2d{2, 2}, sprites["default_big_block"], 2, 4));
    inventory.push_back(std::make_shared<ProgrammableBlock>(olc::vi2d{0, 0}, sprites["code_block"], 1, 1));

//    blocks.push_back(std::make_shared<Block>(olc::vi2d{6, 6}, olc::vi2d{2, 2}, sprites["default_big_block"]));
//    blocks.back()->description = "Big block";
//    blocks.push_back(std::make_shared<ProgrammableBlock>(olc::vi2d{1, 6}, sprites["code_block"]));
//    dynamic_cast<ProgrammableBlock*>(blocks.back().get())->code = "dupa";
//    blocks.back()->description = "Programmable block Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas vehicula massa turpis, in posuere nunc luctus et. Sed non mauris vitae lectus pulvinar rhoncus sit amet sit amet tortor. Donec elit massa, tristique sit amet feugiat et, porttitor ut dolor. Nullam malesuada finibus vehicula. Morbi et efficitur tellus.";
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
        if (this->CursorCollide(block->get()->pos * blocksize, block->get()->size * blocksize, this->GetMousePos()))
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
//    this->DrawPartialSprite(block->pos * blocksize,
//                            block->GetSprite(),
//                            {0, 0},
//                            block->size * (blocksize / spritescale),
//                            spritescale);
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
        ImGui::Begin("Info", NULL, this->infoMenuFlags);
        ImGui::TextWrapped(this->selectedBlock->GetDescription().c_str());
        ImGui::End();
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
        for (auto port = block->get()->ports.begin(); port != block->get()->ports.end(); ++port)
        {
            for (auto conn = port->get()->connections.begin(); conn != port->get()->connections.end(); ++conn)
            {
                this->DrawLine(this->GetBlockCenter(block->get()), this->GetBlockCenter((*conn)->owner), olc::GREEN);
            }
        }
    }
}

void RobotGame::DrawBlocks()
{
    this->SetDrawTarget(this->gridLayer);
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

    this->EnableLayer(gridLayer, gridEnabled);
    this->EnableLayer(connectionsLayer, connEnabled);
    this->EnableLayer(infoUILayer, infoUIEnabled);
    this->EnableLayer(blocksUILayer, blocksUIEnabled);
}

olc::vi2d RobotGame::GetResolution()
{
    int w = this->gridSize.x * this->blocksize + this->infoMenuSize.x;
    int h = std::max(this->gridSize.y * this->blocksize, this->infoMenuSize.y);

    return {w, h};
}

void RobotGame::PlaceBlock(const Block* schema, olc::vi2d pos)
{
    std::shared_ptr<Block> block = std::make_shared<Block>(*schema);
    block->pos = pos;
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
    this->Clear(olc::BLANK);

    this->blocksMenuPos = {0, this->gridSize.y * blocksize};

    int posx = 0;
    for (auto block : inventory)
    {
        this->DrawBlockSprite({posx, this->blocksMenuPos.y}, block.get());
        posx += block->size.x * blocksize + uiPadding;
    }
}

bool RobotGame::OnUserUpdate(float fElapsedTime)
{
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
