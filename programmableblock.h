#ifndef PROGRAMMABLEBLOCK_H
#define PROGRAMMABLEBLOCK_H

#include <string>
#include <memory>
#include "pge/olcPixelGameEngine.h"
#include "lua/lua.hpp"
#include "block.h"
#include "pge/imgui.h"

class ProgrammableBlock : public Block
{
private:
    void InitLua();
    void SetupLuaPorts();
    std::string PopString();
    DataValue GetDataValue(int index);
    void PushDataValue(DataValue data);
    void DisplayError();

    lua_State* L;
    std::string lastError;
    bool running;

    // lua execution timing restrictions
    std::chrono::time_point<std::chrono::steady_clock> prevTime;
    std::chrono::duration<int64_t, std::nano> toYield;
    const std::chrono::milliseconds yieldTime{50};
    const std::chrono::milliseconds warmupTime{300};
    const int hookInstrCount = 100000;
public:
	std::string code;

    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos);
    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, int inputPorts, int outputPorts);
    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts);
    ProgrammableBlock(const ProgrammableBlock& other);
    ~ProgrammableBlock();

    virtual bool IsProgrammable();
    virtual std::string GetDescription();
    virtual olc::Sprite* GetDefaultSprite();
    virtual ProgrammableBlock* Clone();

    bool RunSetup();
    bool RunUpdate();
    bool VerifyCode();
    bool UpdateYieldTimer();
    std::string GetError();
    void Update(float timedelta);
    void Start();
    void Stop();
};

#endif // PROGRAMMABLEBLOCK_H
