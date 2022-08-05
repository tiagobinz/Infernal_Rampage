#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Cor calculada no vértice
in vec3 v_color;
in vec3 v_ambient;
in vec3 v_specular;

// Modelo de iluminação
uniform int lighting_model;

// Modelo de interpolação
uniform int shading_model;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
uniform int object_id;

// Variáveis para acesso das imagens de textura
uniform sampler2D texture_in;

// Variáveis para deslocar a UV
uniform float Uoffset;
uniform float Voffset;

// Variável para eliminar a influência da iluminação sobre este objeto
uniform int unlit;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color_out;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void Phong()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor para modelo de Blinn Phong
    vec4 h = normalize(v + l);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = -l+2*n*(dot(n,l));

    // Coordenadas de textura U e V
    float U = texcoords.x + Uoffset;
    float V = texcoords.y + Voffset;

    // Refletância difusa lida da imagem
    vec3 Kd = texture(texture_in, vec2(U,V)).rgb;

    // Refletância lida da imagem
    float alpha = texture(texture_in, vec2(U,V)).rgba.a;

    if(alpha < 0.5)
        discard;

    vec3 color;

    if (unlit == 0)
    {
        vec3 Ks = vec3(0.0, 0.0, 0.0);
        if(lighting_model == 1)
            Ks = vec3(1.0, 1.0, 1.0);

        // Refletância ambiente
        vec3 Ka = Kd/2;

        // Expoente especular para o modelo de iluminação de Phong
        float q = 32.0f;

        // Espectro da fonte de iluminação
        vec3 I = vec3(1.0,1.0,1.0); // PREENCHA AQUI o espectro da fonte de luz

        // Espectro da luz ambiente
        vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente

        // Termo difuso utilizando a lei dos cossenos de Lambert
        vec3 lambert_diffuse_term = Kd*I*max(0,dot(n,l));

        // Termo ambiente
        vec3 ambient_term = Ka*Ia;

        // Termo especular utilizando o modelo de iluminação de Blinn Phong
        vec3 phong_specular_term  = Ks*I*(pow(max(0,dot(n,h)),q));

        // Cor final do fragmento calculada com uma combinação dos termos difuso,
        // especular, e ambiente. Veja slide 133 do documento "Aula_17_e_18_Modelos_de_Iluminacao.pdf".
        color = lambert_diffuse_term + ambient_term + phong_specular_term;

        // Cor final com correção gamma, considerando monitor sRGB.
        // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
        color = pow(color, vec3(1.0,1.0,1.0)/2.2);
    }
    else
    {
        color = pow(Kd, vec3(1.0,1.0,1.0)/2.2);
    }

    color_out = vec4(color.r, color.g, color.b, alpha);
}

void Gourad()
{
    // Coordenadas de textura U e V
    float U = texcoords.x + Uoffset;
    float V = texcoords.y + Voffset;

    // Refletância difusa lida da imagem
    vec3 Kd = texture(texture_in, vec2(U,V)).rgb;

    // Termo ambiente
    vec3 lambert_diffuse_term = vec3(v_color.r * Kd.r, v_color.g * Kd.g, v_color.b * Kd.b);

    // transparencia lida da imagem
    float alpha = texture(texture_in, vec2(U,V)).rgba.a;

    if(alpha < 0.5)
        discard;

    vec3 color = lambert_diffuse_term + (Kd/2)*vec3(0.2,0.2,0.2) + v_specular;

    color = pow(color, vec3(1.0,1.0,1.0)/2.2);

    color_out = vec4(color.r, color.g, color.b, alpha);
}

void main()
{
    if (shading_model == 0)
        Gourad();

    if (shading_model == 1)
        Phong();
}
