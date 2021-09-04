#include "mapblock.h"

MapBlock::MapBlock(SpriteManager* sm, olc::vi2d pos) : Block(sm, pos, {4, 4}, {"display_input"}, {})
{
    this->SetSprite("screen");
    this->pallete = this->sm->GetSprite("screenpalette");
}

std::string MapBlock::GetDescription()
{
    return "Map display block. Can display 14x14 color picture. "
           "To display something a 100-element table has to be put "
           "on its input port, elements of which should be in range 0-15, "
           "which correspond to different colors. Convenient aliases in the "
           "form of a global table 'color' are provided (e.g. color.red) "
           "which map color names to these numbers";
}

void MapBlock::Start()
{
    this->img = std::shared_ptr<olc::Sprite>(this->img->Duplicate());
}

void MapBlock::Update(float timedelta)
{
    olc::Pixel* spr = this->img->GetData();
    DataValueEx input = this->ports["display_input"]->GetData();
    std::vector<DataValue> inputVec;
    if (std::holds_alternative<std::vector<DataValue>>(input))
        inputVec = std::get<std::vector<DataValue>>(input);
    else
        return;

    std::vector<uint8_t> toDisplay(14*14, 0);
    for (int i = 0; i < inputVec.size(); ++i)
    {
        if (std::holds_alternative<int64_t>(inputVec[i]))
            toDisplay[i] = std::get<int64_t>(inputVec[i]);
    }

    for (int y = 0; y < 56; ++y)
    {
        for (int x = 0; x < 56; ++x)
        {
            uint8_t color = toDisplay[(y / 4) * 14 + (x / 4)] % 16;
            spr[(y + 4) * this->size.x * 16 + (x + 4)] = this->pallete->GetData()[color];
        }
    }
}

void MapBlock::Stop()
{
    this->img = this->GetDefaultSprite();
}
