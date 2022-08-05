#pragma once

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers abaixo são específicos de C++
#include <vector>

#include "ObjModel.h"
#include "GameObject.h"
#include "PlayerController.h"

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada objeto da cena virtual.
struct SceneObject
{
    std::string  name;        // Nome do objeto
    void*        first_index; // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    int          num_indices; // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
};

class World
{
public:
    World();

private:
    // Lista de objetos nomeados, guardados em um dicionário
    std::map<std::string, SceneObject> Meshes;
    // Número de texturas já carregadas na GPU
    int NumLoadedTextures = 0;
    // Mapa para referenciar as texturas carregadas na GPU
    std::map<std::string, int> Textures;
    // Qual é o controlador do jogador?
    PlayerController* PC;
    // Todos objetos do mundo
    std::vector<GameObject*> GameObjects;
    // Objetos que estão esperando para entrar no mundo no próximo ciclo
    std::vector<GameObject*> PendingSpawnObjects;
    // Objetos que estão esperando para serem destruídos no próximo ciclo
    std::vector<GameObject*> PendingKillObjects;
    // Se o mundo devia acabar
    bool ShouldEnd = false;

    // For computing the DeltaSeconds
    float LastTime;

    // Constrói representação de um ObjModel como malha de triângulos para renderização
    void BuildTrianglesAndAddToMeshes(ObjModel* model);
    // Desenha um objeto armazenado em Meshes
    void DrawVirtualObject(const char* object_name);
    // Desenha um objeto do jogo
    void DrawGameObject(GameObject* GObj, float DeltaSeconds, glm::mat4 InMatrix);
    // Lida com objetos que precisam ser criados
    void AddPendingSpawnObjects();
    // Lida com objetos que precisam ser destruídos
    void DestroyPendingKillObjects();

    void ClearScreen();
    void ComputeCameraView();

public:
    // Load 3D mesh from file
    void LoadMesh(const char* filename);
    // Load Texture from file
    void LoadTextureImage(const char* filename, const char* texturename);
    // Draw the entire world on the window
    void DrawWorld(GLFWwindow* window);
    // Qual é o Transform que define a câmera?
    void SetPlayerController(PlayerController* NewPlayerController) { PC = NewPlayerController; }
    // Todos que conhecem o mundo podem saber quem é o jogador
    PlayerController* GetPlayerController() { return PC; }
    // Adds a constructed game object to the world
    void AddToWorld(GameObject* GObj);
    // Destroys a game object
    void Destroy(GameObject* GObj);
    // Faz o mundo acabar
    void End() { ShouldEnd = true; }
    // Retorna todos objetos da cena
    std::vector<GameObject*> GetGameObjects() { return GameObjects; }
    // Indica se o jogo deveria terminar
    bool GetShouldEnd() { return ShouldEnd; }
};
