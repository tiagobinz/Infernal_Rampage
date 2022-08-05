#include "Demon.h"
#include "World.h"
#include "Bullet.h"
#include "Input.h"
//#include "Sound.h"

vector<Path> Demon::AllPaths =
{
    Path
    ({
        vec4(0,3,0,1), vec4(10,5,10,1), vec4(-10,5,5,1), vec4(0,3,0,1)
    }),
    Path
    ({
        vec4(10,3,0,1), vec4(15,5,10,1), vec4(-3,6,5,1), vec4(10,3,0,1)
    }),
    Path
    ({
        vec4(-9,3,0,1), vec4(-15,2,10,1), vec4(3,6,5,1), vec4(-9,3,0,1)
    })
};

Demon::Demon(Transform _ObjectTransform) : GameObject("demon", _ObjectTransform)
{
    MyPath = rand()%3;
    ObjectTransform.Location = AllPaths[MyPath].GetPositionAtTime(0);
    //Sound::Play("../../data/sounds/portal_open3.wav");
}

void Demon::Behavior(float DeltaSeconds)
{
    ObjectTransform.Rotation = GetLookAtRotation(ParentWorld->GetPlayerController()->GetTransform().Location);

    ShootCounter += DeltaSeconds;
    if (ShootCounter >= 5/ShootRate)
    {
        ShootCounter = 0;
        ParentWorld->AddToWorld(new Bullet(ObjectTransform, TEAM_ENEMY));
    }

    Move(DeltaSeconds);
}

void Demon::TakeDamage(int Amount)
{
    Health -= Amount;
    if (Health <= 0)
        Destroy();
}

void Demon::Move(float DeltaSeconds)
{
    auto P = AllPaths[MyPath];

    CurrentPathTime += Speed*DeltaSeconds/50;
    if(CurrentPathTime >= 1)
        CurrentPathTime = CurrentPathTime - 1;

    ObjectTransform.Location = AllPaths[MyPath].GetPositionAtTime(CurrentPathTime);
}
