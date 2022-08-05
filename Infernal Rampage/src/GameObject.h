#pragma once

#include <vector>

#include "Transform.h"
#include "PhysicsEngine.h"

// Lighting models
#define LM_LAMBERT 0
#define LM_BLINN_PHONG 1

// Shading interpolation models
#define SH_GOURAD 0
#define SH_PHONG 1

class World;
struct OverlapResult;

class GameObject
{
public:
    GameObject(const char* _Mesh, Transform _ObjectTransform);
    virtual ~GameObject();

protected:
    Transform ObjectTransform;
    const char *Mesh;
    int ShaderObjectId;
    std::vector<GameObject*> ChildObjects;
    GameObject* ParentObject = nullptr;
    World* ParentWorld = nullptr;
    bool IsPendingKill = false;

public:
    void SetWorld(World* InWorld) { ParentWorld = InWorld; }
    World* GetWorld();
    virtual void Destroy();

    void Update(float DeltaSeconds);
    virtual void Behavior(float DeltaSeconds){}

    glm::vec4 GetForwardVector();
    glm::vec4 GetRightVector();
    glm::vec4 GetUpVector() { return glm::vec4(0,1,0,0); }
    glm::vec3 GetLookAtRotation(glm::vec4 Location);

    float GetDistanceTo(GameObject* Other);

    Transform GetTransform() { return ObjectTransform; }

    const char* GetMesh() { return Mesh; }
    std::vector<GameObject*> GetChildObjects() { return ChildObjects; }
    void AddChildObject(GameObject* Child);
    GameObject* GetParentObject() { return ParentObject; }

    void Translate(glm::vec4 Direction);
    void Rotate(glm::vec3 DeltaRotation);
    void SetRotation(glm::vec3 Rotation) { ObjectTransform.Rotation = Rotation; }
    void SetLocation(glm::vec4 Location) { ObjectTransform.Location = Location; }

    int GetShaderObjectId() { return ShaderObjectId; }

    virtual glm::vec2 GetUVOffset() { return glm::vec2(0,0); }
    virtual int GetLightingModel() { return LM_LAMBERT; }
    virtual int GetShadingModel() { return SH_GOURAD; }
    virtual bool GetIsUnlit() { return false; }
};
