#include "ShaderLoader.h"

// Definições das variáveis estáticas
GLuint ShaderLoader::vertex_shader_id;
GLuint ShaderLoader::fragment_shader_id;
GLuint ShaderLoader::program_id = 0;
GLint ShaderLoader::model_uniform;
GLint ShaderLoader::view_uniform;
GLint ShaderLoader::projection_uniform;
GLint ShaderLoader::object_id_uniform;
GLint ShaderLoader::Uoffset = 0;
GLint ShaderLoader::Voffset = 0;
GLint ShaderLoader::unlit = 0;
GLint ShaderLoader::lighting_model;
GLint ShaderLoader::shading_model;
