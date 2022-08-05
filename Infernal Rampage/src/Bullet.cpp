#include "Bullet.h"
#include "World.h"
#include "Explosion.h"
#include "SphereCollider.h"
#include "PhysicsEngine.h"
#include "Demon.h"
#include "Archdemon.h"
//#include "Sound.h"

Bullet::Bullet(Transform _ObjectTransform, int _Team) : GameObject("bullet", _ObjectTransform)
{
    Direction = GetForwardVector();
    Team = _Team;
}

void Bullet::Behavior(float DeltaSeconds)
{
    Translate(Direction * Speed * DeltaSeconds);
    LifeTimeCounter += DeltaSeconds;
    if (LifeTimeCounter >= LifeTime)
    {
        Destroy();
    }

    ObjectTransform.Rotation = GetLookAtRotation(GetWorld()->GetPlayerController()->GetTransform().Location);

    switch(Team)
    {
    case TEAM_ENEMY:
        CheckCollisionToPlayer();
        break;
    case TEAM_PLAYER:
        CheckCollisionToEnemies();
        break;
    default:
        break;
    }

    if (!IsPendingKill)
    {
        CheckCollisionToGround();
    }
}

void Bullet::CheckCollisionToGround()
{
    auto L = ObjectTransform.Location;
    auto C = PhysicsEngine::CheckPlaneSphere(vec4(0,0,0,1), vec4(0,1,0,0), L, Radius);
    if (C.DidOverlap)
    {
        Destroy();
    }
}

void Bullet::CheckCollisionToPlayer()
{
    PlayerController* P = GetWorld()->GetPlayerController();
    auto C = PhysicsEngine::CheckCapsuleSphere
    (
        P->GetTransform().Location,
        P->GetHalfHeight(),
        P->GetRadius(),
        ObjectTransform.Location,
        Radius
    );
    if(C.DidOverlap)
    {
        P->TakeDamage(1);
        Destroy();
    }
}

void Bullet::CheckCollisionToEnemies()
{
    auto GObjs = GetWorld()->GetGameObjects();
    for (size_t i = 0; i < GObjs.size(); i++)
    {
        auto D = dynamic_cast<Demon*>(GObjs[i]);
        if (D)
        {
            auto C = PhysicsEngine::CheckCapsuleSphere
            (
                D->GetTransform().Location,
                D->GetHalfHeight(),
                D->GetRadius(),
                ObjectTransform.Location,
                Radius
            );
            if(C.DidOverlap)
            {
                D->TakeDamage(1);
                Destroy();
                return;
            }
        }
        else
        {
            auto A = dynamic_cast<Archdemon*>(GObjs[i]);
            if (A)
            {
                auto C = PhysicsEngine::CheckCapsuleSphere
                (
                    A->GetTransform().Location,
                    A->GetHalfHeight(),
                    A->GetRadius(),
                    ObjectTransform.Location,
                    Radius
                );
                if(C.DidOverlap)
                {
                    A->TakeDamage(1);
                    Destroy();
                    return;
                }
            }
        }
    }
}

void Bullet::Hit()
{
    //Sound::Play("../../data/sounds/hit.wav");
    Destroy();
}

void Bullet::Destroy()
{
    GetWorld()->AddToWorld(new Explosion(ObjectTransform));
    GameObject::Destroy();
}
