#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <memory>
#include "pge/olcPixelGameEngine.h"
#include "pge/imgui_impl_pge.h"
#include "pge/imgui_stdlib.h"
#include "lua/lua.hpp"
#include "block.h"
#include "programmableblock.h"
#include "uistate.h"

class IState;

class RobotGame : public olc::PixelGameEngine
{
private:
    olc::imgui::PGE_ImGUI pge_imgui;
    int gameLayer;

    std::vector<std::shared_ptr<olc::Sprite>> sprites;
    std::vector<Block> blocks;

    const int blocksize = 16;
    olc::vi2d gridSize = {16, 9};
    std::string codebuf;
public:
    std::unique_ptr<IState> state;

    RobotGame();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    void DrawGrid();
    Block* GetBlockAt(olc::vi2d pos);
    olc::vi2d GetGridAt(olc::vi2d pos);
    void DrawBlock(Block* block);
    bool CanBePlaced(Block* block, olc::vi2d pos);
    void DrawUI(void);
    void HandleInput();
    void UpdateState();
};

#endif // GAME_H
