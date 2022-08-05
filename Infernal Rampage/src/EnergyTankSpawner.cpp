#include "EnergyTankSpawner.h"
#include "EnergyTank.h"
#include "World.h"

using namespace glm;

EnergyTankSpawner::EnergyTankSpawner() : GameObject("", Transform())
{
}

void EnergyTankSpawner::Behavior(float DeltaSeconds)
{
    auto RandomFloat = [](float a, float b) -> float
    {
        float random = ((float) rand()) / (float) RAND_MAX;
        float diff = b - a;
        float r = random * diff;
        return a + r;
    };

    auto RandomVec = [&RandomFloat](float a, float b) -> vec4
    {
        return vec4(RandomFloat(a,b),.5,RandomFloat(a,b),1);
    };

    SpawnTimer += DeltaSeconds;

    if (SpawnTimer >= SpawnPeriod)
    {
        GetWorld()->AddToWorld(new EnergyTank(Transform(
            RandomVec(-10,10),
            vec3(0,0,0),
            vec3(1,1,1)
        )));

        SpawnTimer = 0;
    }
}
