#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <memory>
#include <filesystem>
#include "pge/olcPixelGameEngine.h"
#include "pge/imgui_impl_pge.h"
#include "pge/imgui_stdlib.h"
#include "lua/lua.hpp"
#include "block.h"
#include "programmableblock.h"
#include "buttonblock.h"
#include "uistate.h"

class IState;

class RobotGame : public olc::PixelGameEngine
{

private:
    olc::imgui::PGE_ImGUI pge_imgui;


    std::vector<std::shared_ptr<Block>> blocks;
    std::vector<std::shared_ptr<Block>> inventory;
    olc::vi2d gridSize = {16, 9};
    olc::vi2d infoMenuPos;
    const olc::vi2d infoMenuSize = {200, 800};
    olc::vi2d blocksMenuPos;
    const olc::vi2d blocksMenuSize = {800, 4 * blocksize};
    const int uiPadding = 8;

    Block* selectedBlock = nullptr;
public:
    std::map<std::string, std::shared_ptr<olc::Sprite>> sprites;
    const int spritesize = 16;                          // one grid unit image size
    const int spritescale = 4;                          // sprite scaling ratio
    const int blocksize = spritesize * spritescale;     // one grid unit rendered size

    const ImGuiWindowFlags codeEditFlags = ImGuiWindowFlags_NoCollapse |
                                           ImGuiWindowFlags_NoNav;

    const ImGuiWindowFlags popUpMenuFlags = ImGuiWindowFlags_NoTitleBar |
                                            ImGuiWindowFlags_NoScrollbar |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoResize |
                                            ImGuiWindowFlags_NoCollapse |
                                            ImGuiWindowFlags_NoNav |
                                            ImGuiWindowFlags_NoBringToFrontOnFocus;

    const ImGuiWindowFlags infoMenuFlags = popUpMenuFlags | ImGuiWindowFlags_NoBackground;

    uint8_t connectionsLayer;
    uint8_t gridLayer;
    uint8_t blocksUILayer;
    uint8_t infoUILayer;
    uint8_t tempLayer;
    uint8_t mapGridLayer;
    std::unique_ptr<IState> state;

    RobotGame();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    void SimTick();

    void DrawGrid();
    olc::vi2d GetResolution();
    Block* GetBlockAt(olc::vi2d pos);
    olc::vi2d GetGridAt(olc::vi2d pos);
    Block* GetBlockUnderMouse();
    Block* GetBlockUnderMouseInv();
    bool CursorCollide(olc::vi2d pos, olc::vi2d size, olc::vi2d mousepos);
    void ClearLayers();
    void DrawBlock(Block* block);
    void DrawBlockSprite(olc::vi2d pos, Block* block);
    void DrawBlocks();
    void DrawBlocksUI();
    void ShowDebugWindow();
    void PlaceBlock(std::shared_ptr<Block> block);
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
