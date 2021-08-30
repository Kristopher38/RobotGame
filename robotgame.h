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
    const olc::vi2d infoUIPos = {1024, 0};

    olc::imgui::PGE_ImGUI pge_imgui;

    std::vector<std::shared_ptr<olc::Sprite>> sprites;
    std::vector<Block> blocks;
    olc::vi2d gridSize = {16, 9};

    Block* selectedBlock = nullptr;
public:
    const int spritesize = 16;                          // one grid unit image size
    const int spritescale = 4;                          // sprite scaling ratio
    const int blocksize = spritesize * spritescale;     // one grid unit rendered size

    uint8_t connectionsLayer;
    uint8_t gridLayer;
    uint8_t blocksUILayer;
    uint8_t infoUILayer;
    std::unique_ptr<IState> state;

    RobotGame();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    void DrawGrid();
    Block* GetBlockAt(olc::vi2d pos);
    olc::vi2d GetGridAt(olc::vi2d pos);
    Block* GetBlockUnderMouse();
    void ClearLayers();
    void DrawBlock(Block* block);
    void DrawBlockSprite(olc::vi2d pos, olc::Sprite* sprite);
    olc::vi2d GetBlockCenter(Block* block);
    void DrawConnections();
    void SelectBlock(Block* block);
    bool CanBePlaced(Block* block, olc::vi2d pos);
    void DrawInfoUI();
    void DrawUI(void);
    void HandleInput();
    void UpdateState();
};

#endif // GAME_H
