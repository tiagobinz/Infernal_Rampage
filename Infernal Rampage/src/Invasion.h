#pragma once

class Invasion
{
public:
    Invasion(){}

    int ArchDemonHealth;
    int DemonHealth;
    int DemonProjectileRate;
    int DemonSpawnRate;
    int DemonSpeed;

    int GetArchDemonDestructivePower()
    {
        return ArchDemonHealth + DemonProjectileRate + DemonSpawnRate + DemonSpeed;
    }
};
