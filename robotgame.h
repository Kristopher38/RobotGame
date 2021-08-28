#ifndef GAME_H
#define GAME_H

#include <vector>
#include "pge/olcPixelGameEngine.h"
#include "pge/imgui_impl_pge.h"
#include "lua/lua.hpp"
#include "block.h"

enum class STATE {IDLE, DRAGGING};

class RobotGame : public olc::PixelGameEngine
{
private:
    olc::imgui::PGE_ImGUI pge_imgui;
    int gameLayer;

    std::vector<std::shared_ptr<olc::Sprite>> sprites;
    std::vector<Block> blocks;
    STATE state = STATE::IDLE;
    const int blocksize = 10;
    olc::vi2d gridSize = {20, 10};

    Block* dragging;
public:
    RobotGame();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    void DrawGrid(int blocksize);
    Block* GetBlockAt(olc::vi2d pos);
    olc::vi2d GetGridAt(olc::vi2d pos);
    void DrawBlock(Block* block);
    bool CanBePlaced(Block* block, olc::vi2d pos);
    void DrawUI(void);
};

#endif // GAME_H
