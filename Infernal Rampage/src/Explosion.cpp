#include "Explosion.h"
#include "World.h"

Explosion::Explosion(Transform _ObjectTransform) : GameObject("explosion", _ObjectTransform)
{
}

void Explosion::Behavior(float DeltaSeconds)
{
    TimePassed += DeltaSeconds;
    ObjectTransform.Rotation = GetLookAtRotation(ParentWorld->GetPlayerController()->GetTransform().Location);
    if (TimePassed >= LifeTime && !IsPendingKill)
    {
        Destroy();
    }
}

glm::vec2 Explosion::GetUVOffset()
{
    float TimeForEachImage = LifeTime/NumberOfImages;
    int TargetImage = (IsPendingKill)? LifeTime/TimeForEachImage - 1 : TimePassed/TimeForEachImage;
    return glm::vec2
    (
        (TargetImage%4) *  0.25,
        (TargetImage/4) * -0.25
    );
}
