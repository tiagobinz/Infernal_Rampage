#include "World.h"
#include "BasicOpenGlStuff.h"
#include "ShaderLoader.h"
#include <algorithm>
#include <iostream>

// For reading images
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

World::World()
{
    LastTime = glfwGetTime();
}

void World::AddToWorld(GameObject* GObj)
{
    if (GObj)
    {
        PendingSpawnObjects.push_back(GObj);
        GObj->SetWorld(this);
    }
}

void World::Destroy(GameObject* GObj)
{
    if (std::find(GameObjects.begin(), GameObjects.end(), GObj) != GameObjects.end())
    {
        PendingKillObjects.push_back(GObj);
    }
}

void World::DrawWorld(GLFWwindow* window)
{
    // Preenche o framebuffer com a cor de fundo
    ClearScreen();

    // Adicionamos à lista de processamento os objetos que foram instanciados no último ciclo
    AddPendingSpawnObjects();

    // Destruímos todos objetos que ficaram pendentes para serem destruídos
    DestroyPendingKillObjects();

    // Calculamos a variação de tempo desde o último ciclo
    float DeltaSeconds = glfwGetTime() - LastTime;
    LastTime = glfwGetTime();

    // Processamos o comportamento de todos objetos que estão no mundo
    for (size_t i = 0; i < GameObjects.size(); i++)
    {
        GameObjects[i]->Update(DeltaSeconds);
        for (size_t j = 0; j < GameObjects[i]->GetChildObjects().size(); j++)
            GameObjects[i]->GetChildObjects()[j]->Update(DeltaSeconds);
    }

    // Computa a visão câmera
    ComputeCameraView();

    // Desenhamos todos objetos que estão no mundo
    for (size_t i = 0; i < GameObjects.size(); i++)
    {
        DrawGameObject(GameObjects[i], DeltaSeconds, Matrix_Identity());
    }

    // Troca dos buffers, mostrando para o usuário tudo que foi renderizado pelas funções acima.
    glfwSwapBuffers(window);

    // Lidamos com callbacks do sistema operacional
    glfwPollEvents();

    if(ShouldEnd)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void World::ClearScreen()
{
    // Definimos a cor do "fundo" do framebuffer como branco.
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

    // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
    // e também resetamos todos os pixels do Z-buffer (depth buffer).
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void World::ComputeCameraView()
{
    if (!PC)
        printf("ERROR: You have to set the player controller!\n");

    glm::vec4 camera_position_c  = PC->GetTransform().Location;
    glm::vec4 camera_view_vector = PC->GetForwardVector();
    glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f);

    // Computamos a matriz "View" utilizando os parâmetros da câmera para definir o sistema de coordenadas da câmera.
    glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

    // Enviamos as matrizes "view" e "projection" para a placa de vídeo (GPU).
    // Veja o arquivo "shader_vertex.glsl", onde estas são efetivamente aplicadas em todos os pontos.
    glUniformMatrix4fv(ShaderLoader::view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(ShaderLoader::projection_uniform , 1 , GL_FALSE , glm::value_ptr(PC->GetProjectionMatrix(BasicOpenGlStuff::ScreenRatio)));
}

void World::DrawGameObject(GameObject* GObj, float DeltaSeconds, glm::mat4 InMatrix)
{
    Transform T = GObj->GetTransform();
    InMatrix =
            InMatrix
          * Matrix_Translate(T.Location.x, T.Location.y, T.Location.z)
          * Matrix_Rotate_Z(T.GetRads().z)
          * Matrix_Rotate_Y(T.GetRads().y)
          * Matrix_Rotate_X(T.GetRads().x)
          * Matrix_Scale(T.Scale.x,T.Scale.y,T.Scale.z);
    if (GObj->GetMesh())
    {
        glUniformMatrix4fv(ShaderLoader::model_uniform, 1 , GL_FALSE , glm::value_ptr(InMatrix));
        glUniform1i(ShaderLoader::object_id_uniform, 0);
        auto UVO = GObj->GetUVOffset();
        glUniform1f(ShaderLoader::Uoffset, UVO.x);
        glUniform1f(ShaderLoader::Voffset, UVO.y);
        glUniform1i(ShaderLoader::unlit, GObj->GetIsUnlit()? 1 : 0);
        glUniform1i(ShaderLoader::lighting_model, GObj->GetLightingModel());
        glUniform1i(ShaderLoader::shading_model, GObj->GetShadingModel());
        DrawVirtualObject(GObj->GetMesh());
    }
    for (size_t i = 0; i < GObj->GetChildObjects().size(); i++)
    {
        DrawGameObject(GObj->GetChildObjects()[i], DeltaSeconds, InMatrix);
    }
}

void World::AddPendingSpawnObjects()
{
    for (size_t i = 0; i < PendingSpawnObjects.size(); i++)
    {
        GameObjects.push_back(PendingSpawnObjects[i]);
    }
    PendingSpawnObjects.clear();
}

void World::DestroyPendingKillObjects()
{
    for (size_t i = 0; i < PendingKillObjects.size(); i++)
    {
        unsigned j = 0;
        bool found = false;
        while(!found && j < GameObjects.size())
        {
            if (GameObjects[j] == PendingKillObjects[i])
            {
                GameObjects.erase(GameObjects.begin()+j);
                found = true;
            }
            j++;
        }
        delete PendingKillObjects[i];
    }
    PendingKillObjects.clear();
}

void World::LoadMesh(const char* filename)
{
    // Construímos a representação de objetos geométricos através de malhas de triângulos
    ObjModel model(filename);
    BuildTrianglesAndAddToMeshes(&model);
}

void World::LoadTextureImage(const char* filename, const char* texturename)
{
    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    if ( data == NULL )
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    // A textura se repete em tiles
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Parâmetros de amostragem da textura. Falaremos sobre eles em uma próxima aula.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    GLuint textureunit = NumLoadedTextures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);

    stbi_image_free(data);

    printf("OK (%dx%d) index %d.\n", width, height, textureunit);

    Textures[texturename] = textureunit;

    NumLoadedTextures += 1;
}

// Função que desenha um objeto armazenado em Meshes. Veja definição
// dos objetos na função BuildTrianglesAndAddToVirtualScene().
void World::DrawVirtualObject(const char* object_name)
{
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(Meshes[object_name].vertex_array_object_id);

    // Setamos a textura
    glUniform1i(glGetUniformLocation(ShaderLoader::program_id, "texture_in"), Textures[object_name]);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // Meshes[""] dentro da função BuildTrianglesAndAddToVirtualScene(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        Meshes[object_name].rendering_mode,
        Meshes[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)Meshes[object_name].first_index
    );

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

// Constrói triângulos para futura renderização a partir de um ObjModel.
void World::BuildTrianglesAndAddToMeshes(ObjModel* model)
{
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    std::vector<GLuint> indices;
    std::vector<float>  model_coefficients;
    std::vector<float>  normal_coefficients;
    std::vector<float>  texture_coefficients;

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t first_index = indices.size();
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];

                indices.push_back(first_index + 3*triangle + vertex);

                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                //printf("tri %d vert %d = (%.2f, %.2f, %.2f)\n", (int)triangle, (int)vertex, vx, vy, vz);
                model_coefficients.push_back( vx ); // X
                model_coefficients.push_back( vy ); // Y
                model_coefficients.push_back( vz ); // Z
                model_coefficients.push_back( 1.0f ); // W

                // Inspecionando o código da tinyobjloader, o aluno Bernardo
                // Sulzbach (2017/1) apontou que a maneira correta de testar se
                // existem normais e coordenadas de textura no ObjModel é
                // comparando se o índice retornado é -1. Fazemos isso abaixo.

                if ( idx.normal_index != -1 )
                {
                    const float nx = model->attrib.normals[3*idx.normal_index + 0];
                    const float ny = model->attrib.normals[3*idx.normal_index + 1];
                    const float nz = model->attrib.normals[3*idx.normal_index + 2];
                    normal_coefficients.push_back( nx ); // X
                    normal_coefficients.push_back( ny ); // Y
                    normal_coefficients.push_back( nz ); // Z
                    normal_coefficients.push_back( 0.0f ); // W
                }

                if ( idx.texcoord_index != -1 )
                {
                    const float u = model->attrib.texcoords[2*idx.texcoord_index + 0];
                    const float v = model->attrib.texcoords[2*idx.texcoord_index + 1];
                    texture_coefficients.push_back( u );
                    texture_coefficients.push_back( v );
                }
            }
        }

        size_t last_index = indices.size() - 1;

        SceneObject theobject;
        theobject.name           = model->shapes[shape].name;
        theobject.first_index    = (void*)first_index; // Primeiro índice
        theobject.num_indices    = last_index - first_index + 1; // Número de indices
        theobject.rendering_mode = GL_TRIANGLES;       // Índices correspondem ao tipo de rasterização GL_TRIANGLES.
        theobject.vertex_array_object_id = vertex_array_object_id;

        Meshes[model->shapes[shape].name] = theobject;
    }

    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if ( !normal_coefficients.empty() )
    {
        GLuint VBO_normal_coefficients_id;
        glGenBuffers(1, &VBO_normal_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
        location = 1; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if ( !texture_coefficients.empty() )
    {
        GLuint VBO_texture_coefficients_id;
        glGenBuffers(1, &VBO_texture_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
        location = 2; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}
