#pragma once

#include "GameObject.h"
#include "Invasion.h"
#include "Path.h"

class Archdemon : public GameObject
{
public:
    Archdemon(Transform _T);

private:
    // Spawn lesser demons
    float SpawnCounter = 0;
    void SpawnDemon();

    // Death sequence
    bool IsDying = false;
    float DeathTime;
    float DeathExplosionCounter = 0;
    void DeathBehavior(float DeltaSeconds);

    // Movement
    Path DemonPath = Path
    ({
        vec4(0,3,15,1), vec4(-40,3,0,1), vec4(0,3,-15,1), vec4(40,3,0,1), vec4(0,3,15,1)
    });
    float CurrentPathTime = 0;
    void Move(float DeltaSeconds);

public:
    virtual void Behavior(float DeltaSeconds);
    bool GetIsDying() { return IsDying; }
    float GetHalfHeight() { return 1; }
    float GetRadius() { return 1; }
    void TakeDamage(int Amount);

    // Invasion stuff
    int Health = 10;
    int LesserDemonsHealth = 3;
    int LesserDemonsShootRate = 1;
    float LesserDemonsSpeed = 5;
    float SpawnRate = 2;
};
