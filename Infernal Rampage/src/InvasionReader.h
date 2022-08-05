#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Invasion.h"

class InvasionReader
{
private:
    static Invasion* CurrentInvasion;

    static int ReadNumber(FILE* f);

public:
    static Invasion* GetCurrentInvasion() { return CurrentInvasion; }

    static void ReadInvasion(char* FileName);
};
