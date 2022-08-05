#include "AssaultRifle.h"
#include "Bullet.h"
#include "World.h"
#include "Sound.h"

AssaultRifle::AssaultRifle(Transform _ObjectTransform) : GameObject("assault_rifle", _ObjectTransform)
{
    StartingLocation = ObjectTransform.Location;

    Counter1 = new Counter
    (
        Transform
        (
            0.0033f,
            0,
            0
        )
    ); AddChildObject(Counter1);
    Counter2 = new Counter
    (
        Transform
        (
            -0.0033f,
            0,
            0
        )
    ); AddChildObject(Counter2);

    UpdateCounters();
}


void AssaultRifle::Fire(float DeltaSeconds)
{
    FireTimer += DeltaSeconds;

    if (FireTimer >= FirePeriod && BulletCount > 0)
    {
        FireTimer = 0;
        if (GetWorld() && ParentObject)
        {
            auto T = ParentObject->GetTransform();
            auto BulletTransform = Transform(T.Location + ParentObject->GetForwardVector()*0.5f, T.Rotation, T.Scale);
            GetWorld()->AddToWorld(new Bullet(BulletTransform, TEAM_PLAYER));
            PlayFireSound();
            AddBullets(-1);
        }
    }
}

void AssaultRifle::PlayFireSound()
{
    int sound = rand()%3;
    if (sound == 0)
        Sound::Play("../../data/sounds/ar_fire_a.wav");
    else if (sound == 1)
        Sound::Play("../../data/sounds/ar_fire_b.wav");
    else
        Sound::Play("../../data/sounds/ar_fire_c.wav");
}

void AssaultRifle::NotFire()
{
    FireTimer = FirePeriod;
}

void AssaultRifle::Walk(float DeltaSeconds)
{
    WalkAnimationTime += DeltaSeconds * WalkAnimationSpeed;
    if (WalkAnimationTime > PI*2)
    {
        WalkAnimationTime -= PI*2;
    }

    ObjectTransform.Location = StartingLocation + glm::vec4(0,sin(WalkAnimationTime) * WalkAnimationAmplitude,0,0);
}

void AssaultRifle::NotWalk(float DeltaSeconds)
{
    if (fabs(sin(WalkAnimationTime)) > 0.1f)
    {
        Walk(DeltaSeconds);
    }
}

void AssaultRifle::UpdateCounters()
{
    int n = BulletCount;
    Counter2->SetNumber(n%10);
    n/=10;
    Counter1->SetNumber(n%10);
}

void AssaultRifle::AddBullets(int Count)
{
    BulletCount += Count;
    if (BulletCount > 99)
        BulletCount = 99;
    UpdateCounters();
}
