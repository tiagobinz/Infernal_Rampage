#pragma once

#include "InvasionReader.h"
#define RESULT_VICTORY 1
#define RESULT_DEFEAT 0

struct GameResults
{
    float PlayTime;
    int VictoryOrDefeat;

    int GetScore()
    {
        auto I = InvasionReader::GetCurrentInvasion();
        auto D = I->GetArchDemonDestructivePower();
        auto minutes = PlayTime/60;
        return ((float)D)/minutes;
    }
};
