#include "radarblock.h"

std::string RadarBlock::GetDescription()
{
    return "Radar block. Allows you to query a specific map position "
           "for info by providing an area and offset relative to the robot's "
           "position on the position_input input in the form of "
           "four-element table {x, y, w, h} where x and y are the offset and "
           "can be any integers in the range -7 to 7, w and h are width "
           "and height of the area and can be any integers from 1 to 5 "
           "(they get clamped to these values). "
           "IMPORTANT: the value returned from the radar for a specific "
           "query (in the form of a table of w*h size containing string "
           "representations of the environment, e.g. \"wall\", \"floor\", \"item\") "
           "will be available at the data_output output on the NEXT update() call.";
}

static int clamp(int v, int lo, int hi)
{
    if (v < lo)
        return lo;
    else if (v > hi)
        return hi;
    else
        return v;
}

void RadarBlock::Update(float timedelta)
{
    DataValueEx data = this->ports["position_input"]->GetData();

    if (std::holds_alternative<std::vector<DataValue>>(data))
    {
        std::vector<DataValue> dataVec = std::get<std::vector<DataValue>>(data);
        std::vector<int64_t> values(4, 0);
        int i = 0;
        for (auto val : dataVec)
        {
            if (std::holds_alternative<int64_t>(val))
                values[i] = std::get<int64_t>(val);
            i++;
        }
        int x = clamp(values[0], -7, 7);
        int y = clamp(values[1], -7, 7);
        int w = clamp(values[2], 1, 5);
        int h = clamp(values[3], 1, 5);

        std::vector<DataValue> output;

        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                std::string atPos = "floor";
                olc::vi2d curPos = {robot->pos.x + x + j, robot->pos.y + y + i};
                if (curPos == robot->pos)
                    atPos = "robot";
                else if (this->map->GetPos("walls", curPos))
                    atPos = "wall";
                else if (this->map->GetPos("items", curPos))
                    atPos = "item";
                output.push_back(atPos);
            }
        }
        this->ports["data_output"]->Update(output);
    }
    else
        return;
}
