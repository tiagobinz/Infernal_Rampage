#pragma once

#include "GameObject.h"

class EnergyTank : public GameObject
{
private:
    float Radius = .5f;

public:
    EnergyTank(Transform T);

    virtual void Behavior(float DeltaSeconds) override;

    virtual int GetShadingModel() override { return SH_PHONG; }
};
