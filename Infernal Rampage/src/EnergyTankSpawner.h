#pragma once

#include "GameObject.h"

class EnergyTankSpawner : public GameObject
{
public:
    EnergyTankSpawner();

    virtual void Behavior(float DeltaSeconds) override;

private:
    float SpawnPeriod = 15;
    float SpawnTimer = 0;
};
