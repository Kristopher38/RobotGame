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
    DataValueEx GetDataValue(int index);
    void PushDataValue(DataValueEx data);
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

    const std::vector<std::string> colors = {
        "black",
        "blue",
        "light_blue",
        "violet",
        "pink",
        "green",
        "light_green",
        "orange",
        "yellow",
        "red",
        "light_red",
        "brown",
        "light_brown",
        "gray",
        "light_gray",
        "white"
    };

    const std::string defaultCode = "-- write code that runs once here\n\nfunction update()\n-- write code that runs every simulation tick here\nend";
public:
	std::string code;

    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos);
    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, int inputPorts, int outputPorts);
    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts);
    ProgrammableBlock(const ProgrammableBlock& other);
    ~ProgrammableBlock();

    virtual bool IsProgrammable();
    virtual std::string GetDescription();
    virtual std::shared_ptr<olc::Sprite> GetDefaultSprite();
    virtual ProgrammableBlock* Clone();

    bool RunSetup();
    bool RunUpdate();
    bool VerifyCode();
    bool UpdateYieldTimer();
    void ResetYieldTimer(std::chrono::milliseconds t);
    void SetRunning(bool val);
    std::string GetError();
    void Update(float timedelta);
    void Start();
    void Stop();
};

#endif // PROGRAMMABLEBLOCK_H
