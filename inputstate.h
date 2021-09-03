#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "pge/olcPixelGameEngine.h"

struct InputState
{
	olc::HWButton lmb;
	olc::HWButton rmb;
	olc::HWButton mmb;
	olc::vi2d mousepos;

	olc::HWButton* kbd;
};

#endif // INPUTSTATE_H
