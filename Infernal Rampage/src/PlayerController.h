#pragma once

#include "GameObject.h"
#include "AssaultRifle.h"
#include "Demon.h"

class PlayerController : public GameObject
{
public:
    PlayerController(Transform _ObjectTransform);

private:
    // Camera configuration
    const float NearPlane = -0.01f;
    const float FarPlane  = -1000.0f;
    const float FOV = 1.0471973;

    // Weapon reference
    AssaultRifle* AR;

    // Lock-in mechanic
    bool DoingLockIn;
    Demon* CurrentLockTarget = nullptr;
    void StartLockIn();
    void StopLockIn();

    // Player input functions called in Update
    void GetMovementInput(float DeltaSeconds);
    void GetLookInput(float DeltaSeconds);
    void GetFireInput(float DeltaSeconds);

    // Input variables
    const float LookSensitivity = 5.0f;
    float WalkingSpeed = 3.0f;

    // Health
    int Health = 10;

    // Collision stuff
    float Radius = .5f;
    float HalfHeight = .5f;

public:
    glm::mat4 GetProjectionMatrix(float ScreenRatio);

    virtual void Behavior(float DeltaSeconds) override;

    void AddBullets(int Count);
    void AddHealth(int Amount);

    float GetRadius() { return Radius; }
    float GetHalfHeight() { return HalfHeight; }

    void TakeDamage(int Amount);
    void Die();

    void CheckCollisionToLava();
};
