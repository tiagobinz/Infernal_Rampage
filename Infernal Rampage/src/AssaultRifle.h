#pragma once

#include "GameObject.h"
#include "Counter.h"

class AssaultRifle : public GameObject
{
public:
    AssaultRifle(Transform _ObjectTransform);

private:
    // Gunfire variables
    const float FirePeriod = 0.1f;
    float FireTimer = FirePeriod;
    int BulletCount = 99;

    // Walking animation variables
    const float WalkAnimationAmplitude = 0.004f;
    const float WalkAnimationSpeed = 8;
    float WalkAnimationTime = 0;
    glm::vec4 StartingLocation;

    // Bullet counter variables
    Counter* Counter1;
    Counter* Counter2;

    void UpdateCounters();

    void PlayFireSound();

public:
    void Fire(float DeltaSeconds);
    void NotFire();

    void Walk(float DeltaSeconds);
    void NotWalk(float DeltaSeconds);

    void AddBullets(int Count);

    virtual int GetLightingModel() { return LM_BLINN_PHONG; }
    virtual int GetShadingModel() { return SH_PHONG; }
};
