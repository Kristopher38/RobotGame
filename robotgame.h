#ifndef GAME_H
#define GAME_H

#include <vector>
#include "olcPixelGameEngine.h"
#include "lua/lua.hpp"
#include "block.h"

enum class STATE {IDLE, DRAGGING};

class RobotGame : public olc::PixelGameEngine
{
private:
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
};

#endif // GAME_H
