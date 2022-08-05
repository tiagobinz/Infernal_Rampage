#include "World.h"

class Map
{
private:
    static World* GameWorld;

    static void LoadAssets();
    static void MakePlayer(float x, float z);
    static void MakeGround();
    static void MakeRock(float x, float z, float rot);
    static void MakeSky();

public:
    static void Create(World* _GameWorld);
};
