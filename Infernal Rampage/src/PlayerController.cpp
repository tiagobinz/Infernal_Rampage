#include "PlayerController.h"
#include "matrices.h"
#include "Input.h"
#include "World.h"
#include "Bullet.h"
#include "Sound.h"

#include <algorithm>

PlayerController::PlayerController(Transform _ObjectTransform) : GameObject("", _ObjectTransform)
{
    AR = new AssaultRifle
    (
        Transform
        (
            glm::vec4(-0.1f,-0.1365f,0.05f, 1.0f),
            glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(1.0f,1.0f,1.0f)
        )
    );
    AddChildObject(AR);
}

glm::mat4 PlayerController::GetProjectionMatrix(float ScreenRatio)
{
    return Matrix_Perspective(FOV, ScreenRatio, NearPlane, FarPlane);
}

void PlayerController::Behavior(float DeltaSeconds)
{
    GetMovementInput(DeltaSeconds);
    GetLookInput(DeltaSeconds);
    GetFireInput(DeltaSeconds);

    if (!IsPendingKill)
        CheckCollisionToLava();
}

void PlayerController::GetMovementInput(float DeltaSeconds)
{
    bool Walking = false;
    // Walk using W A S D
    if (Input::GetKey(GLFW_KEY_W))
    {
        glm::vec4 Movement = GetForwardVector();
        Movement.y = 0;
        Translate(Movement * DeltaSeconds * WalkingSpeed);
        Walking = true;
    }
    if (Input::GetKey(GLFW_KEY_S))
    {
        glm::vec4 Movement = -GetForwardVector();
        Movement.y = 0;
        Translate(Movement * DeltaSeconds * WalkingSpeed);
        Walking = true;
    }
    if (Input::GetKey(GLFW_KEY_D))
    {
        Translate(GetRightVector() * DeltaSeconds * WalkingSpeed);
        Walking = true;
    }
    if (Input::GetKey(GLFW_KEY_A))
    {
        Translate(-GetRightVector() * DeltaSeconds * WalkingSpeed);
        Walking = true;
    }

    if (Walking)
    {
        AR->Walk(DeltaSeconds);
    }
    else
    {
        AR->NotWalk(DeltaSeconds);
    }
}

void PlayerController::GetLookInput(float DeltaSeconds)
{
    if(Input::GetMouseButton(GLFW_MOUSE_BUTTON_2))
    {
        if(!DoingLockIn)
        {
            StartLockIn();
        }
    }
    else
    {
        if (DoingLockIn)
        {
            StopLockIn();
        }
    }

    if(DoingLockIn)
    {
        if (CurrentLockTarget)
        {
            ObjectTransform.Rotation = GetLookAtRotation(CurrentLockTarget->GetTransform().Location);
        }
    }
    else
    {
        // Rotate the character using the mouse input
        Rotate
        (
            glm::vec3
            (
                Input::GetMouseDelta().y,
                -Input::GetMouseDelta().x,
                0.0f
            ) * DeltaSeconds * LookSensitivity
        );
    }


    // Lambda function to clamp a value
    auto Clamp = [](float v, float lo, float hi) -> float
    {
        if (v > hi) return hi;
        if (v < lo) return lo;
        return v;
    };
    // Limit the X rotation to [-90, 90]
    glm::vec3 R = ObjectTransform.Rotation;
    SetRotation
    (
        glm::vec3
        (
            Clamp(R.x, -90, 90),
            R.y,
            0.0f
        )
    );
}

void PlayerController::StartLockIn()
{
    DoingLockIn = true;

    // All game objects
    auto G = GetWorld()->GetGameObjects();

    // Potential targets
    std::vector<Demon*> T;

    // Look for demons in sight
    for (size_t i = 0; i < G.size(); i++)
    {
        // Check if it is a demon
        auto D = dynamic_cast<Demon*>(G[i]);
        if (D)
        {
            // Lock in direction
            auto L = D->GetTransform().Location - ObjectTransform.Location;
            L /= norm(L);

            // Look vector
            auto F = GetForwardVector();

            // If angle is small enough
            if (acos(dotproduct(F,L)) < PI/8)
            {
                // It is a potential target
                T.push_back(D);
            }
        }
    }

    float Closest = 10000;
    int Chosen = -1;
    // Check the targets
    for (size_t i = 0; i < T.size(); i++)
    {
        // Distance
        auto D = (T[i]->GetTransform().Location - ObjectTransform.Location).length();
        if (D < Closest)
        {
            Closest = D;
            Chosen = i;
        }
    }

    if (Chosen >= 0)
    {
        CurrentLockTarget = T[Chosen];
    }
    else
    {
        StopLockIn();
    }
}

void PlayerController::StopLockIn()
{
    DoingLockIn = false;
    CurrentLockTarget = nullptr;
}

void PlayerController::GetFireInput(float DeltaSeconds)
{
    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_1))
    {
        AR->Fire(DeltaSeconds);
    }
    else
    {
        AR->NotFire();
    }
}

void PlayerController::AddBullets(int Count)
{
    AR->AddBullets(Count);
}

void PlayerController::AddHealth(int Amount)
{
    Health += Amount;
}

void PlayerController::TakeDamage(int Amount)
{
    Health -= Amount;
    Sound::Play("../../data/sounds/pain4.wav");
    if (Health <= 0)
    {
        Die();
    }
}

void PlayerController::Die()
{
    GetWorld()->End();
}

void PlayerController::CheckCollisionToLava()
{
// This is how we check a face.
#define PHYS_PLANE(b,n) (PhysicsEngine::CheckCapsulePlane(ObjectTransform.Location, HalfHeight, Radius, b, n))
#define CHECK(b,n) if(PHYS_PLANE(b,n).DidOverlap){Die();return;}
#define LAVA_DIST 13

    // Check the collision to 4 invisible faces!

    // Face A
    vec4 ab = vec4(LAVA_DIST,0,0,1);
    vec4 an = vec4(-1,0,0,0);
    CHECK(ab,an);

    // Face B
    vec4 bb = vec4(-LAVA_DIST,0,0,1);
    vec4 bn = vec4(1,0,0,0);
    CHECK(bb,bn);

    // Face C
    vec4 cb = vec4(0,0,LAVA_DIST,1);
    vec4 cn = vec4(0,0,-1,0);
    CHECK(cb,cn);

    // Face D
    vec4 db = vec4(0,0,-LAVA_DIST,1);
    vec4 dn = vec4(0,0,1,0);
    CHECK(db,dn);

#undef PHYS_PLANE
#undef CHECK
#undef LAVA_DIST
}
