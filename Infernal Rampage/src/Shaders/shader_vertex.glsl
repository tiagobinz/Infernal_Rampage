#version 330 core

// Atributos de v�rtice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a fun��o BuildTrianglesAndAddToVirtualScene()
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no c�digo C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Modelo de interpola��o
uniform int shading_model;
// Modelo de ilumina��o
uniform int lighting_model;
// Vari�vel para eliminar a influ�ncia da ilumina��o sobre este objeto
uniform int unlit;

// Atributos de v�rtice que ser�o gerados como sa�da ("out") pelo Vertex Shader.
// ** Estes ser�o interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais ser�o recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;
out vec3 v_color;
out vec3 v_specular;

void Gourad()
{
    // Obtemos a posi��o da c�mera utilizando a inversa da matriz que define o
    // sistema de coordenadas da c�mera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual � coberto por um ponto que percente � superf�cie de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posi��o no
    // sistema de coordenadas global (World coordinates). Esta posi��o � obtida
    // atrav�s da interpola��o, feita pelo rasterizador, da posi��o de cada
    // v�rtice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada v�rtice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em rela��o ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Vetor que define o sentido da c�mera em rela��o ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor para modelo de Blinn Phong
    vec4 h = normalize(v + l);

    // Vetor que define o sentido da reflex�o especular ideal.
    vec4 r = -l+2*n*(dot(n,l));

    // Reflet�ncia difusa
    v_color = vec3(1,1,1);
    v_specular = vec3(0,0,0);

    if (unlit == 0)
    {
        vec3 Ks = vec3(0.0, 0.0, 0.0);
        if(lighting_model == 1)
            Ks = vec3(1.0, 1.0, 1.0);

        // Expoente especular para o modelo de ilumina��o de Phong
        float q = 32.0f;

        // Espectro da fonte de ilumina��o
        vec3 I = vec3(1.0,1.0,1.0);

        // Termo difuso utilizando a lei dos cossenos de Lambert
        vec3 lambert_diffuse_term = vec3(1,1,1)*I*max(0,dot(n,l));

        // Termo especular utilizando o modelo de ilumina��o de Blinn Phong
        vec3 phong_specular_term  = Ks*I*(pow(max(0,dot(n,h)),q));

        // Cor final do fragmento calculada com uma combina��o dos termos difuso,
        // especular, e ambiente. Veja slide 133 do documento "Aula_17_e_18_Modelos_de_Iluminacao.pdf".
        v_color = lambert_diffuse_term;

        v_specular = phong_specular_term;
    }
}

void main()
{
    // A vari�vel gl_Position define a posi��o final de cada v�rtice
    // OBRIGATORIAMENTE em "normalized device coordinates" (NDC), onde cada
    // coeficiente estar� entre -1 e 1 ap�s divis�o por w.
    // Veja slides 144 e 150 do documento "Aula_09_Projecoes.pdf".
    //
    // O c�digo em "main.cpp" define os v�rtices dos modelos em coordenadas
    // locais de cada modelo (array model_coefficients). Abaixo, utilizamos
    // opera��es de modelagem, defini��o da c�mera, e proje��o, para computar
    // as coordenadas finais em NDC (vari�vel gl_Position). Ap�s a execu��o
    // deste Vertex Shader, a placa de v�deo (GPU) far� a divis�o por W. Veja
    // slide 189 do documento "Aula_09_Projecoes.pdf".

    gl_Position = projection * view * model * model_coefficients;

    // Como as vari�veis acima  (tipo vec4) s�o vetores com 4 coeficientes,
    // tamb�m � poss�vel acessar e modificar cada coeficiente de maneira
    // independente. Esses s�o indexados pelos nomes x, y, z, e w (nessa
    // ordem, isto �, 'x' � o primeiro coeficiente, 'y' � o segundo, ...):
    //
    //     gl_Position.x = model_coefficients.x;
    //     gl_Position.y = model_coefficients.y;
    //     gl_Position.z = model_coefficients.z;
    //     gl_Position.w = model_coefficients.w;
    //

    // Agora definimos outros atributos dos v�rtices que ser�o interpolados pelo
    // rasterizador para gerar atributos �nicos para cada fragmento gerado.

    // Posi��o do v�rtice atual no sistema de coordenadas global (World).
    position_world = model * model_coefficients;

    // Posi��o do v�rtice atual no sistema de coordenadas local do modelo.
    position_model = model_coefficients;

    // Normal do v�rtice atual no sistema de coordenadas global (World).
    // Veja slide 107 do documento "Aula_07_Transformacoes_Geometricas_3D.pdf".
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;

    // Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
    texcoords = texture_coefficients;

    // GOURAD
    if (shading_model == 0)
    {
        Gourad();
    }
}

