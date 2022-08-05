#pragma once

#include "GameObject.h"
#include "Path.h"

class Demon : public GameObject
{
public:
    Demon(Transform _ObjectTransform);

    virtual void Behavior(float DeltaSeconds) override;

private:
    // Shooting
    float ShootCounter = 0;

    // Collision
    float Radius = .5f;
    float HalfHeight = .5f;

    // Movement
    static vector<Path> AllPaths;
    float CurrentPathTime = 0;
    int MyPath;

public:
    float GetRadius() { return Radius; }
    float GetHalfHeight() { return HalfHeight; }

    void TakeDamage(int Amount);

    void Move(float DeltaSeconds);

    // Life
    int Health = 3;
    float ShootRate = 1;
    float Speed = 5;
};
