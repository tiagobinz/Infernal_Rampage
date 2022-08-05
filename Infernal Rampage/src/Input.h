// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers abaixo são específicos de C++
#include <map>

class Input
{
private:
    // Statics that store the input information
    //static std::map<int,bool> KeysDown;
    //static std::map<int,bool> KeysUp;
    static std::map<int,bool> Keys;
    //static std::map<int,bool> MouseButtonsUp;
    //static std::map<int,bool> MouseButtonsDown;
    static std::map<int,bool> MouseButtons;
    static glm::vec2 MouseDelta;
    static glm::vec2 LastCursorPosition;
    static bool JustStartedCursor;
    static double LastCursorPositionInputTime;

    // Callbacks
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
    // Listen to the inputs given to the window
    static void Setup(GLFWwindow* window);

    // Boolean functions:
    // Keyboard
    //static bool GetKeyDown(int key);
    //static bool GetKeyUp(int key);
    static bool GetKey(int key) { return Keys[key]; }
    // Mouse
    //static bool GetMouseButtonDown(int button);
    //static bool GetMouseButtonUp(int button);
    static bool GetMouseButton(int button) { return MouseButtons[button]; }
    static glm::vec2 GetMouseDelta();
};
