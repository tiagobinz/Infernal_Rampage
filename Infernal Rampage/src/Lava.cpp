#include "Lava.h"

Lava::Lava(Transform _ObjectTransform) : GameObject("lava", _ObjectTransform)
{
}

void Lava::Behavior(float DeltaSeconds)
{
    LavaUVLoop += DeltaSeconds/20;
    if (LavaUVLoop > 1)
        LavaUVLoop = LavaUVLoop - 1;
}

glm::vec2 Lava::GetUVOffset()
{
    return glm::vec2(LavaUVLoop, LavaUVLoop);
}
