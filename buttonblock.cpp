#include "buttonblock.h"
#include "robotgame.h"

std::string ButtonBlock::GetDescription()
{
    return "Button block. Returns 0 or 1 on its output depending on whether it's being pressed or not";
}

ButtonBlock* ButtonBlock::Clone()
{
    return new ButtonBlock(*this);
}

void ButtonBlock::HandleInput(RobotGame* game)
{
    olc::HWButton lmb = game->GetMouse(olc::Mouse::LEFT);
    if (lmb.bHeld)
    {
        this->SetSprite(game->sprites["button_pressed"]);
        this->ports["btn_value"]->Update(1);
    }
    else
    {
        this->SetSprite(game->sprites["button"]);
        this->ports["btn_value"]->Update(0);
    }
}
