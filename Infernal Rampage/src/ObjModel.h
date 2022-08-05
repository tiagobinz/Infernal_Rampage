#pragma once

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Cria��o de contexto OpenGL 3.3

// Headers da biblioteca GLM: cria��o de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"

// Estrutura que representa um modelo geom�trico carregado a partir de um
// arquivo ".obj". Veja https://en.wikipedia.org/wiki/Wavefront_.obj_file .
struct ObjModel
{
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;

    // Este construtor l� o modelo de um arquivo utilizando a biblioteca tinyobjloader.
    // Veja: https://github.com/syoyo/tinyobjloader
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true)
    {
        printf("Carregando modelo \"%s\"... ", filename);

        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

        if (!err.empty())
            fprintf(stderr, "\n%s\n", err.c_str());

        if (!ret)
            throw std::runtime_error("Erro ao carregar modelo.");

        printf("OK.\n");
        ComputeNormals();
    }

    void ComputeNormals()
    {
        if (!attrib.normals.empty())
            return;

        // Primeiro computamos as normais para todos os TRI�NGULOS.
        // Segundo, computamos as normais dos V�RTICES atrav�s do m�todo proposto
        // por Gouraud, onde a normal de cada v�rtice vai ser a m�dia das normais de
        // todas as faces que compartilham este v�rtice.

        size_t num_vertices = attrib.vertices.size() / 3;

        std::vector<int> num_triangles_per_vertex(num_vertices, 0);
        std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f,0.0f,0.0f,0.0f));

        for (size_t shape = 0; shape < shapes.size(); ++shape)
        {
            size_t num_triangles = shapes[shape].mesh.num_face_vertices.size();

            for (size_t triangle = 0; triangle < num_triangles; ++triangle)
            {
                assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

                glm::vec4  vertices[3];
                for (size_t vertex = 0; vertex < 3; ++vertex)
                {
                    tinyobj::index_t idx = shapes[shape].mesh.indices[3*triangle + vertex];
                    const float vx = attrib.vertices[3*idx.vertex_index + 0];
                    const float vy = attrib.vertices[3*idx.vertex_index + 1];
                    const float vz = attrib.vertices[3*idx.vertex_index + 2];
                    vertices[vertex] = glm::vec4(vx,vy,vz,1.0);
                }

                const glm::vec4  a = vertices[0];
                const glm::vec4  b = vertices[1];
                const glm::vec4  c = vertices[2];

                // PREENCHA AQUI o c�lculo da normal de um tri�ngulo cujos v�rtices
                // est�o nos pontos "a", "b", e "c", definidos no sentido anti-hor�rio.
                const glm::vec4  n = crossproduct(b-a,c-a);

                for (size_t vertex = 0; vertex < 3; ++vertex)
                {
                    tinyobj::index_t idx = shapes[shape].mesh.indices[3*triangle + vertex];
                    num_triangles_per_vertex[idx.vertex_index] += 1;
                    vertex_normals[idx.vertex_index] += n;
                    shapes[shape].mesh.indices[3*triangle + vertex].normal_index = idx.vertex_index;
                }
            }
        }

        attrib.normals.resize( 3*num_vertices );

        for (size_t i = 0; i < vertex_normals.size(); ++i)
        {
            glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
            n /= norm(n);
            attrib.normals[3*i + 0] = n.x;
            attrib.normals[3*i + 1] = n.y;
            attrib.normals[3*i + 2] = n.z;
        }
    }
};
