#include "Archdemon.h"
#include "Demon.h"
#include "World.h"
#include "Explosion.h"
#include "Sound.h"

#include <time.h>
#include <random>

Archdemon::Archdemon(Transform T) : GameObject("archdemon_1", T)
{
    DeathTime = 0;
    AddChildObject(new GameObject("archdemon_2",Transform()));
    ObjectTransform.Location = DemonPath.GetPositionAtTime(0);
}

void Archdemon::Behavior(float DeltaSeconds)
{
    if (!IsDying)
    {
        SpawnCounter += DeltaSeconds;

        if (SpawnCounter >= 8/SpawnRate)
        {
            SpawnCounter = 0;
            srand(time(0));
            SpawnDemon();
        }
    }
    else
    {
        DeathBehavior(DeltaSeconds);
    }

    if (!IsDying)
    {
        ObjectTransform.Rotation = GetLookAtRotation(vec4(0,ObjectTransform.Location.y,0,1));
        Move(DeltaSeconds);
    }
}

void Archdemon::SpawnDemon()
{
    auto D = new Demon(Transform());
    D->Health = LesserDemonsHealth;
    D->ShootRate = LesserDemonsShootRate;
    D->Speed = LesserDemonsSpeed;
    GetWorld()->AddToWorld(D);
}

float random(int lo, int hi) //range : [min, max)
{
   static bool first = true;
   if (first)
   {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   int ret = lo + rand() % (( hi + 1 ) - lo);
   return ret;
}

void Archdemon::DeathBehavior(float DeltaSeconds)
{
    DeathTime += DeltaSeconds;
    DeathExplosionCounter += DeltaSeconds;

    if (DeathExplosionCounter > .5f)
    {
        DeathExplosionCounter = 0;
        auto L = ObjectTransform.Location + glm::vec4(random(-10,10)/10,random(-20,20)/10,random(-10,10)/10,0);
        auto R = ObjectTransform.Rotation;
        auto S = ObjectTransform.Scale;
        GetWorld()->AddToWorld(new Explosion(Transform(L,R,S)));
    }

    if (DeathTime >= 5)
    {
        GetWorld()->End();
    }
}

void Archdemon::Move(float DeltaSeconds)
{
    CurrentPathTime += DeltaSeconds/10;

    if(CurrentPathTime >= 1)
        CurrentPathTime -= 1;

    ObjectTransform.Location = DemonPath.GetPositionAtTime(CurrentPathTime);
}

void Archdemon::TakeDamage(int Amount)
{
    Health -= Amount;
    if (Health <= 0 && !IsDying)
    {
        Sound::Play("../../data/sounds/death_01.wav");
        IsDying = true;
    }
}
