//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//             Éder Matheus e Tiago Binz
//                 INFERNAL RAMPAGE

#include "BasicOpenGlStuff.h"
#include "ShaderLoader.h"
#include "Input.h"
#include "World.h"
#include "Map.h"
#include "GameResults.h"
#include "GameTexts.h"
#include "Archdemon.h"
#include "Sound.h"

#include <time.h>

#define CLEAR_CONSOLE() system("CLS")

// The game's most high level functions are in this file
void GameIntro();
GameResults PlayGame();
void Feedback(GameResults Results);
int CheckVictory(World* W);

int main()
{
    // Introduction
    GameIntro();

    // Game loop
    GameResults Results = PlayGame();

    // Victory / Defeat
    Feedback(Results);

    return 0;
}

void GameIntro()
{
    InvasionReader::ReadInvasion((char*)"../../config/DemonInvasion.txt");
    GameTexts::IntroText();
    fflush(stdin);
    getchar();
    CLEAR_CONSOLE();
}

GameResults PlayGame()
{
    // Aqui serão armazendos os resultados da partida
    GameResults ReturnValue;

    // Criamos a janela do jogo
    GLFWwindow* window = BasicOpenGlStuff::CreateOpenGlWindow((char*)"Infernal Rampage", 1200, 7ssss00);

    // Configuramos os inputs
    Input::Setup(window);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados para renderização.
    ShaderLoader::LoadShadersFromFiles();

    // Criamos o mundo do jogo
    World* GameWorld = new World();

    // Carregamos os modelos e construímos o cenário
    Map::Create(GameWorld);

    // Vamos utilizar isso para determinar o tempo total de jogo
    auto GameStartTime = glfwGetTime();

    // Inicializamos a semente de aleatório
    srand(time(0));

    // Inicializamos o motor de som
    Sound::Initialize();

    // Começa a música do jogo
    Sound::PlayLoop("../../data/sounds/bensound-highoctane.wav");

    // Loop do jogo
    while (!glfwWindowShouldClose(window))
    {
        GameWorld->DrawWorld(window);
    }

    ReturnValue.PlayTime = glfwGetTime() - GameStartTime;
    ReturnValue.VictoryOrDefeat = CheckVictory(GameWorld);

    // Fim da partida
    return ReturnValue;
}

int CheckVictory(World* W)
{
    Archdemon* A;
    auto GObjs = W->GetGameObjects();
    for (size_t i = 0; i < GObjs.size(); i++)
    {
        auto Cast = dynamic_cast<Archdemon*>(GObjs[i]);
        if (Cast)
        {
            A = Cast;
            break;
        }
    }

    if (A)
    {
        return A->GetIsDying()? RESULT_VICTORY : RESULT_DEFEAT;
    }
    else
    {
        return RESULT_DEFEAT;
    }
}

void Feedback(GameResults Results)
{
    CLEAR_CONSOLE();
    auto RecievedInvasion = InvasionReader::GetCurrentInvasion();

    if (Results.VictoryOrDefeat == RESULT_VICTORY)
        GameTexts::VictoryText(Results);
    else
        GameTexts::FailureText(Results);

    GameTexts::GameStatisticsText(RecievedInvasion, Results);
}
