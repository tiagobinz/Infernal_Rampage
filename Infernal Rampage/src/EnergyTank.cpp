#include "EnergyTank.h"
#include "PhysicsEngine.h"
#include "World.h"
//#include "Sound.h"

EnergyTank::EnergyTank(Transform T) : GameObject("energy_tank", T)
{
}

void EnergyTank::Behavior(float DeltaSeconds)
{
    ObjectTransform.Rotation.y += DeltaSeconds * 30;

    auto P = GetWorld()->GetPlayerController();
    auto C = PhysicsEngine::CheckCapsuleSphere
    (
        P->GetTransform().Location,
        P->GetHalfHeight(),
        P->GetRadius(),
        ObjectTransform.Location,
        Radius
    );
    if (C.DidOverlap && !IsPendingKill)
    {
        P->AddBullets(50);
        //Sound::Play("../../data/sounds/energy_charge.wav");
        Destroy();
    }
}
