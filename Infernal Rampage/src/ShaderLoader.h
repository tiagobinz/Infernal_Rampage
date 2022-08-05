#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <glad/glad.h>   // Cria��o de contexto OpenGL 3.3

class ShaderLoader
{
public:
    // Vari�veis que definem um programa de GPU (shaders). Veja fun��o LoadShadersFromFiles().
    static GLuint vertex_shader_id;
    static GLuint fragment_shader_id;
    static GLuint program_id;
    static GLint model_uniform;
    static GLint view_uniform;
    static GLint projection_uniform;
    static GLint object_id_uniform;

    // Para modelos de luz
    static GLint lighting_model;
    static GLint shading_model;

    // Para deslocar a UV
    static GLint Uoffset;
    static GLint Voffset;

    // Para eliminar a influ�ncia da luz
    static GLint unlit;

    // Fun��o que carrega os shaders de v�rtices e de fragmentos que ser�o utilizados para renderiza��o
    static void LoadShadersFromFiles()
    {
        vertex_shader_id = LoadShader_Vertex("../../src/Shaders/shader_vertex.glsl");
        fragment_shader_id = LoadShader_Fragment("../../src/Shaders/shader_fragment.glsl");

        // Deletamos o programa de GPU anterior, caso ele exista.
        if ( program_id != 0 )
            glDeleteProgram(program_id);

        // Criamos um programa de GPU utilizando os shaders carregados acima.
        program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

        // Buscamos o endere�o das vari�veis definidas dentro do Vertex Shader.
        // Utilizaremos estas vari�veis para enviar dados para a placa de v�deo
        // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
        model_uniform           = glGetUniformLocation(program_id, "model"); // Vari�vel da matriz "model"
        view_uniform            = glGetUniformLocation(program_id, "view"); // Vari�vel da matriz "view" em shader_vertex.glsl
        projection_uniform      = glGetUniformLocation(program_id, "projection"); // Vari�vel da matriz "projection" em shader_vertex.glsl
        object_id_uniform       = glGetUniformLocation(program_id, "object_id"); // Vari�vel "object_id" em shader_fragment.glsl
        Uoffset                 = glGetUniformLocation(program_id, "Uoffset");
        Voffset                 = glGetUniformLocation(program_id, "Voffset");
        unlit                   = glGetUniformLocation(program_id, "unlit");
        lighting_model          = glGetUniformLocation(program_id, "lighting_model");
        shading_model           = glGetUniformLocation(program_id, "shading_model");

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de v�rtice e fragmentos).
        glUseProgram(program_id);
    }

    // Carrega um Vertex Shader de um arquivo GLSL. Veja defini��o de LoadShader() abaixo.
    static GLuint LoadShader_Vertex(const char* filename)
    {
        // Criamos um identificador (ID) para este shader, informando que o mesmo
        // ser� aplicado nos v�rtices.
        GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

        // Carregamos e compilamos o shader
        LoadShader(filename, vertex_shader_id);

        // Retorna o ID gerado acima
        return vertex_shader_id;
    }

    // Carrega um Fragment Shader de um arquivo GLSL . Veja defini��o de LoadShader() abaixo.
    static GLuint LoadShader_Fragment(const char* filename)
    {
        // Criamos um identificador (ID) para este shader, informando que o mesmo
        // ser� aplicado nos fragmentos.
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        // Carregamos e compilamos o shader
        LoadShader(filename, fragment_shader_id);

        // Retorna o ID gerado acima
        return fragment_shader_id;
    }

    // Fun��o auxilar, utilizada pelas duas fun��es acima. Carrega c�digo de GPU de
    // um arquivo GLSL e faz sua compila��o.
    static void LoadShader(const char* filename, GLuint shader_id)
    {
        // Lemos o arquivo de texto indicado pela vari�vel "filename"
        // e colocamos seu conte�do em mem�ria, apontado pela vari�vel
        // "shader_string".
        std::ifstream file;
        try {
            file.exceptions(std::ifstream::failbit);
            file.open(filename);
        } catch ( std::exception& e ) {
            fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
            std::exit(EXIT_FAILURE);
        }
        std::stringstream shader;
        shader << file.rdbuf();
        std::string str = shader.str();
        const GLchar* shader_string = str.c_str();
        const GLint   shader_string_length = static_cast<GLint>( str.length() );

        // Define o c�digo do shader GLSL, contido na string "shader_string"
        glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

        // Compila o c�digo do shader GLSL (em tempo de execu��o)
        glCompileShader(shader_id);

        // Verificamos se ocorreu algum erro ou "warning" durante a compila��o
        GLint compiled_ok;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos mem�ria para guardar o log de compila��o.
        // A chamada "new" em C++ � equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];
        glGetShaderInfoLog(shader_id, log_length, &log_length, log);

        // Imprime no terminal qualquer erro ou "warning" de compila��o
        if ( log_length != 0 )
        {
            std::string  output;

            if ( !compiled_ok )
            {
                output += "ERROR: OpenGL compilation of \"";
                output += filename;
                output += "\" failed.\n";
                output += "== Start of compilation log\n";
                output += log;
                output += "== End of compilation log\n";
            }
            else
            {
                output += "WARNING: OpenGL compilation of \"";
                output += filename;
                output += "\".\n";
                output += "== Start of compilation log\n";
                output += log;
                output += "== End of compilation log\n";
            }

            fprintf(stderr, "%s", output.c_str());
        }

        // A chamada "delete" em C++ � equivalente ao "free()" do C
        delete [] log;
    }

    // Esta fun��o cria um programa de GPU, o qual cont�m obrigatoriamente um
    // Vertex Shader e um Fragment Shader.
    static GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
    {
        // Criamos um identificador (ID) para este programa de GPU
        GLuint program_id = glCreateProgram();

        // Defini��o dos dois shaders GLSL que devem ser executados pelo programa
        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Linkagem dos shaders acima ao programa
        glLinkProgram(program_id);

        // Verificamos se ocorreu algum erro durante a linkagem
        GLint linked_ok = GL_FALSE;
        glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

        // Imprime no terminal qualquer erro de linkagem
        if ( linked_ok == GL_FALSE )
        {
            GLint log_length = 0;
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

            // Alocamos mem�ria para guardar o log de compila��o.
            // A chamada "new" em C++ � equivalente ao "malloc()" do C.
            GLchar* log = new GLchar[log_length];

            glGetProgramInfoLog(program_id, log_length, &log_length, log);

            std::string output;

            output += "ERROR: OpenGL linking of program failed.\n";
            output += "== Start of link log\n";
            output += log;
            output += "\n== End of link log\n";

            // A chamada "delete" em C++ � equivalente ao "free()" do C
            delete [] log;

            fprintf(stderr, "%s", output.c_str());
        }

        // Os "Shader Objects" podem ser marcados para dele��o ap�s serem linkados
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        // Retornamos o ID gerado acima
        return program_id;
    }
};
