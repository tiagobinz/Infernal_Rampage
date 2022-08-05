#include "GameObject.h"
#include "matrices.h"
#include "World.h"

GameObject::GameObject(const char* _Mesh, Transform _ObjectTransform)
{
    Mesh = _Mesh;
    ObjectTransform = _ObjectTransform;
}

GameObject::~GameObject()
{
    delete Mesh;
}

glm::vec4 GameObject::GetForwardVector()
{
    glm::vec3 Rads = ObjectTransform.GetRads();
	return glm::vec4
	(
        cos(Rads.x)*sin(Rads.y),
        -sin(Rads.x),
        cos(Rads.x)*cos(Rads.y),
        0.0f
    );
}

glm::vec4 GameObject::GetRightVector()
{
    glm::vec3 Rads = ObjectTransform.GetRads();
    Rads.x = 0.0f;
    Rads.y -= PI/2;
    return glm::vec4
	(
        cos(Rads.x)*sin(Rads.y),
        -sin(Rads.x),
        cos(Rads.x)*cos(Rads.y),
        0.0f
    );
}

glm::vec3 GameObject::GetLookAtRotation(glm::vec4 Location)
{
    glm::vec4 Dir = glm::normalize(Location - ObjectTransform.Location);

    float y = acos
    (
        dotproduct
        (
            glm::normalize(glm::vec4(Dir.x,0.0f,Dir.z,0.0f)),
            glm::vec4(0.0f,0.0f,1.0f,0.0f)
        )
    ) * 57.2958f;   // Para angulos em graus

    if (Location.x < ObjectTransform.Location.x)
    {
        y *= -1;
    }

    auto DirNoY = glm::normalize(glm::vec4(Dir.x,0.0f,Dir.z,0.0f));
    float x = 0;
    if (DirNoY.y != Dir.y)
    {
        x = acos
        (
            dotproduct
            (
                Dir,
                glm::normalize(glm::vec4(Dir.x,0.0f,Dir.z,0.0f))
            )
        ) * 57.2958f;   // Para angulos em graus

        if (Location.y > ObjectTransform.Location.y)
        {
            x *= -1;
        }
    }

    return glm::vec3(x,y,0.0f);
}

void GameObject::Update(float DeltaSeconds)
{
    if (!IsPendingKill)
        Behavior(DeltaSeconds);
}

float GameObject::GetDistanceTo(GameObject* Other)
{
    glm::vec4 Dif = ObjectTransform.Location - Other->ObjectTransform.Location;
    return Dif.length();
}

void GameObject::Translate(glm::vec4 Direction)
{
    ObjectTransform.Location += Direction;
}

void GameObject::Rotate(glm::vec3 DeltaRotation)
{
    ObjectTransform.Rotation += DeltaRotation;
}

void GameObject::AddChildObject(GameObject* Child)
{
    ChildObjects.push_back(Child);
    Child->ParentObject = this;
}

World* GameObject::GetWorld()
{
    if (ParentWorld != nullptr)
    {
        return ParentWorld;
    }
    else if (ParentObject != nullptr)
    {
        return ParentObject->GetWorld();
    }
    else
    {
        return nullptr;
    }
}

void GameObject::Destroy()
{
    IsPendingKill = true;
    if (GetWorld())
    {
        GetWorld()->Destroy(this);
    }
}
