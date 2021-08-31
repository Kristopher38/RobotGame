#include "programmableblock.h"

ProgrammableBlock::ProgrammableBlock(const ProgrammableBlock& other) : Block(other)
{
    this->code = other.code;
    this->L = nullptr;
}

bool ProgrammableBlock::IsProgrammable()
{
    return true;
}

std::string ProgrammableBlock::GetDescription()
{
    return "Programmable block. Can be programmed in Lua to receive and respond to inputs "
           "from other components (e.g. buttons) and produce outputs which can be sent for "
           "further processing to another programmable blocks or connect to other components "
           "to control them.";
}

ProgrammableBlock* ProgrammableBlock::Clone()
{
    return new ProgrammableBlock(*this);
}
