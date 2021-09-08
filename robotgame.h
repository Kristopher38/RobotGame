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
#include "diodeblock.h"
#include "motorblock.h"
#include "mapblock.h"
#include "radarblock.h"
#include "uistate.h"
#include "inputstate.h"
#include "map.h"
#include "robot.h"

class IState;

class RobotGame : public olc::PixelGameEngine
{

private:
    olc::imgui::PGE_ImGUI pge_imgui;
    Map map;
    Robot robot;

    std::vector<std::shared_ptr<Block>> blocks;
    olc::vi2d gridSize = {16, 9};
    olc::vi2d infoMenuPos;
    const olc::vi2d infoMenuSize = {200, 776};
    olc::vi2d blocksMenuPos;
    const olc::vi2d blocksMenuSize = {800, 2 * blocksize + 20};
    const int uiPadding = 8;

    Block* selectedBlock = nullptr;
public:

    std::vector<std::pair<int, std::shared_ptr<Block>>> inventory;
    float timedelta;
    const int tps = 60;
    SpriteManager sm;
    const int spritesize = 16;                          // one grid unit image size
    const int spritescale = 4;                          // sprite scaling ratio
    const int blocksize = spritesize * spritescale;     // one grid unit rendered size
    std::map<std::string, std::string> helpFiles;
    const std::filesystem::path examplesPath = {"./examples"};

    const ImGuiWindowFlags codeEditFlags = ImGuiWindowFlags_NoCollapse |
                                           ImGuiWindowFlags_NoNav |
                                           ImGuiWindowFlags_NoScrollWithMouse;

    const ImGuiWindowFlags popUpMenuFlags = ImGuiWindowFlags_NoTitleBar |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoResize |
                                            ImGuiWindowFlags_NoCollapse |
                                            ImGuiWindowFlags_NoNav |
                                            ImGuiWindowFlags_NoBringToFrontOnFocus;

    const ImGuiWindowFlags infoMenuFlags = popUpMenuFlags | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar;

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

    void DrawHelp();
    void DrawGrid();
    olc::vi2d GetResolution();
    Block* GetBlockAt(olc::vi2d pos);
    olc::vi2d GetGridAt(olc::vi2d pos);
    Block* GetBlockUnderMouse();
    Block* GetBlockUnderMouseInv();
    void IncrInvCount(const Block* schema, int count);
    bool CursorCollide(olc::vi2d pos, olc::vi2d size, olc::vi2d mousepos);
    void ClearLayers();
    void DrawBlock(Block* block);
    void DrawBlockSprite(olc::vi2d pos, Block* block, int scale = -1);
    void DrawBlocks();
    void DrawBlocksUI();
    void ShowDebugWindow();
    void PlaceBlock(std::shared_ptr<Block> block);
    void RemoveBlock(Block* block);
    int InvCount(const Block* schema);
    olc::vi2d GetBlockCenter(Block* block);
    void DrawConnections();
    void SelectBlock(Block* block);
    bool CanBePlaced(Block* block, olc::vi2d pos);
    void DrawInfoUI();
    void DrawUI(void);
    void HandleInput();
    void UpdateState();
    void SimStart();
    void SimStop();
};

#endif // GAME_H
