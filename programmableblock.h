#ifndef PROGRAMMABLEBLOCK_H
#define PROGRAMMABLEBLOCK_H

#include <string>
#include "block.h"

class ProgrammableBlock : public Block
{
public:
	std::string code;
    ProgrammableBlock();
};

#endif // PROGRAMMABLEBLOCK_H
