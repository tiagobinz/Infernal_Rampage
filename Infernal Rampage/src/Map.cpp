#include "Map.h"
#include "PlayerController.h"
#include "Demon.h"
#include "Lava.h"
#include "Explosion.h"
#include "Sky.h"
#include "Archdemon.h"
#include "EnergyTankSpawner.h"
#include "EnergyTank.h"
#include "InvasionReader.h"

World* Map::GameWorld;

void Map::LoadAssets()
{

    // Carregamos os modelos que serão utilizados no jogo
    GameWorld->LoadMesh("../../data/plane.obj");
    GameWorld->LoadMesh("../../data/demon.obj");
    GameWorld->LoadMesh("../../data/assault_rifle.obj");
    GameWorld->LoadMesh("../../data/counter.obj");
    GameWorld->LoadMesh("../../data/bullet.obj");
    GameWorld->LoadMesh("../../data/rock.obj");
    GameWorld->LoadMesh("../../data/lava.obj");
    GameWorld->LoadMesh("../../data/explosion.obj");
    GameWorld->LoadMesh("../../data/sky.obj");
    GameWorld->LoadMesh("../../data/archdemon_1.obj");
    GameWorld->LoadMesh("../../data/archdemon_2.obj");
    GameWorld->LoadMesh("../../data/energy_tank.obj");

    // Carregamos as texturas que serão utilizadas no jogo
    // Os nomes dados às texturas precisam ser exatamente o nome da mesh
    GameWorld->LoadTextureImage("../../data/assault_rifle.png", "assault_rifle");
    GameWorld->LoadTextureImage("../../data/demon.png", "demon");
    GameWorld->LoadTextureImage("../../data/counter.png", "counter");
    GameWorld->LoadTextureImage("../../data/bullet.png", "bullet");
    GameWorld->LoadTextureImage("../../data/plane.jpg", "plane");
    GameWorld->LoadTextureImage("../../data/rock.jpg", "rock");
    GameWorld->LoadTextureImage("../../data/lava.jpg", "lava");
    GameWorld->LoadTextureImage("../../data/explosion.png", "explosion");
    GameWorld->LoadTextureImage("../../data/sky.png", "sky");
    GameWorld->LoadTextureImage("../../data/archdemon_1.png", "archdemon_1");
    GameWorld->LoadTextureImage("../../data/archdemon_2.png", "archdemon_2");
    GameWorld->LoadTextureImage("../../data/energy_tank.png", "energy_tank");
}

void Map::MakePlayer(float x, float z)
{
    // Criamos o player
    PlayerController* PC = new PlayerController(Transform(x,1,z));
    GameWorld->AddToWorld(PC);

    // E dizemos ao mundo que a posição do player define a posição da câmera
    GameWorld->SetPlayerController(PC);
}

void Map::MakeGround()
{
    // Criamos o plano do chão
    GameWorld->AddToWorld
    (
        new GameObject
        (
            "plane",
            Transform
            (
                glm::vec4(0.0f,0.0f,0.0f,1.0f),
                glm::vec3(0.0f,0.0f,0.0f),
                glm::vec3(1.0f,1.0f,1.0f)
            )
        )
    );

    // Criamos a lava
    GameWorld->AddToWorld
    (
        new Lava
        (
            Transform
            (
                glm::vec4(0.0f,0.0f,0.0f,1.0f),
                glm::vec3(0.0f,0.0f,0.0f),
                glm::vec3(1.0f,1.0f,1.0f)
            )
        )
    );
}

void Map::MakeRock(float x, float z, float rot)
{
    // Criamos o plano do chão
    GameWorld->AddToWorld
    (
        new GameObject
        (
            "rock",
            Transform
            (
                glm::vec4(x,0,z,1),
                glm::vec3(0.0f,rot,0.0f),
                glm::vec3(1.0f,1.0f,1.0f)
            )
        )
    );
}

void Map::MakeSky()
{
    GameWorld->AddToWorld(new Sky());
}

void Map::Create(World* _GameWorld)
{
    GameWorld = _GameWorld;
    LoadAssets();
    MakePlayer(-2.0f,0.0f);
    MakeGround();
    MakeSky();

    // Criamos todas pedras
    /*MakeRock(-4,-3,0);
    MakeRock(2,-4,25);
    MakeRock(8,-1,50);
    MakeRock(-1,10,75);
    MakeRock(-6,6,14);
    MakeRock(-9,-8,54);
    MakeRock(7,-10,67);
    MakeRock(9,9,90);
    MakeRock(-9,9,45);*/

    // Criamos o demônio chefe
    auto I = InvasionReader::GetCurrentInvasion();
    auto A = new Archdemon(Transform());
    A->Health = I->ArchDemonHealth;
    A->LesserDemonsHealth = I->DemonHealth;
    A->LesserDemonsShootRate = I->DemonSpawnRate;
    A->SpawnRate = I->DemonSpawnRate;
    A->LesserDemonsSpeed = I->DemonSpeed;

    GameWorld->AddToWorld(A);

    // Criamos o criador de caixas de munição
    GameWorld->AddToWorld(new EnergyTankSpawner());
}
