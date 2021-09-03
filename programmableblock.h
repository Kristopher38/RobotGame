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

    lua_State* L;
    std::string lastError;
    bool running;

    // lua execution timing restrictions
    double timedelta;
    double toYield;
    const double yieldTime = 0.05; // 50ms
    const double warmupTime = 0.3; // 300ms
public:
	std::string code;

    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos);
    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, int inputPorts, int outputPorts);
    ProgrammableBlock(SpriteManager* sm, olc::vi2d pos, std::vector<std::string> inputPorts, std::vector<std::string> outputPorts);
    ProgrammableBlock(const ProgrammableBlock& other);
    ~ProgrammableBlock();

    virtual bool IsProgrammable();
    virtual std::string GetDescription();
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
