#pragma once

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

#define PI 3.14159f
#define PI_180 PI/180

struct Transform
{
    glm::vec4 Location;
    glm::vec3 Rotation;
    glm::vec3 Scale;

    Transform()
    {
        Location = glm::vec4(0.0f,0.0f,0.0f,1.0f);
        Rotation = glm::vec3(0.0f,0.0f,0.0f);
        Scale = glm::vec3(1.0f,1.0f,1.0f);
    }

    Transform(glm::vec4 _Location, glm::vec3 _Rotation, glm::vec3 _Scale)
    {
        Location = _Location;
        Rotation = _Rotation;
        Scale = _Scale;
    }

    Transform(float x, float y, float z) : Transform(glm::vec4(x,y,z,1.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f))
    {
    }

    glm::vec3 GetRads()
    {
        return glm::vec3
        (
             Rotation.x * PI_180,
             Rotation.y * PI_180,
             Rotation.z * PI_180
        );
    }

    void Print()
    {
        printf("Location: ");
        printf("%f ", Location.x);
        printf("%f ", Location.y);
        printf("%f \n", Location.z);
        printf("Rotation: ");
        printf("%f ", Rotation.x);
        printf("%f ", Rotation.y);
        printf("%f \n", Rotation.z);
        printf("Scale: ");
        printf("%f ", Scale.x);
        printf("%f ", Scale.y);
        printf("%f \n", Scale.z);
    }
};
