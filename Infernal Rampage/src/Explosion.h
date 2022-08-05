#pragma once

#include "GameObject.h"

class Explosion : public GameObject
{
public:
    Explosion(Transform _ObjectTransform);

private:
    const float LifeTime = 1;
    const int NumberOfImages = 16;
    float TimePassed = 0;

public:
    virtual void Behavior(float DeltaSeconds) override;
    virtual glm::vec2 GetUVOffset() override;
    virtual bool GetIsUnlit() override { return true; }
};
