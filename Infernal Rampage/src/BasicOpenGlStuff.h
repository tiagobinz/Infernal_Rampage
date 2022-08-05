// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

class BasicOpenGlStuff
{
public:
    // Screen proportions, width and height
    static float ScreenRatio;

    // Create a window
    static GLFWwindow* CreateOpenGlWindow(char* title, int width, int height);

    // Errors
    static void ErrorCallback(int error, const char* description);

    // Frame buffer size
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
