#pragma once

#include "GameObject.h"

#define TEAM_PLAYER 0
#define TEAM_ENEMY  1

class Bullet : public GameObject
{
public:
    Bullet(Transform _ObjectTransform, int _Team);

    virtual void Behavior(float DeltaSeconds) override;

private:
    const float Speed = 10.0f;
    const float LifeTime = 2.0f;
    float LifeTimeCounter = 0.0f;

    int Team;

    glm::vec4 Direction;

    float Radius = .2f;
    void CheckCollisionToPlayer();
    void CheckCollisionToEnemies();
    void CheckCollisionToGround();

    void Hit();

public:
    virtual bool GetIsUnlit() override { return true; }

    virtual void Destroy() override;
};
