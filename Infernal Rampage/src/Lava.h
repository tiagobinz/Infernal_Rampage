#pragma once

#include "GameObject.h"

class Lava : public GameObject
{
public:
    Lava(Transform _ObjectTransform);

private:
    float LavaUVLoop = 0;

public:
    virtual void Behavior(float DeltaSeconds) override;
    virtual glm::vec2 GetUVOffset() override;
};
