#include "Input.h"

//std::map<int,bool> Input::KeysDown;
//std::map<int,bool> Input::KeysUp;
std::map<int,bool> Input::Keys;
//std::map<int,bool> Input::MouseButtonsUp;
//std::map<int,bool> Input::MouseButtonsDown;
std::map<int,bool> Input::MouseButtons;
glm::vec2 Input::MouseDelta;
glm::vec2 Input::LastCursorPosition;
bool Input::JustStartedCursor = true;
double Input::LastCursorPositionInputTime;

void Input::Setup(GLFWwindow* window)
{
    // Configure the callbacks
    glfwSetKeyCallback(window, Input::KeyCallback);
    glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
    glfwSetCursorPosCallback(window, Input::CursorPosCallback);
    glfwSetScrollCallback(window, Input::ScrollCallback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    // Initialize statics
    // Keyboard
    for (int i = GLFW_KEY_UNKNOWN; i <= GLFW_KEY_MENU; i++)
    {
        //KeysDown[i] = false;
        //KeysUp[i] = false;
        Keys[i] = false;
    }
    // Mouse
    for (int i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_LAST; i++)
    {
        //MouseButtonsDown[i] = false;
        //MouseButtonsUp[i] = false;
        MouseButtons[i] = false;
    }
    MouseDelta = glm::vec2(0.0f,0.0f);
    LastCursorPosition = glm::vec2(0.0f,0.0f);
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (action == GLFW_REPEAT || action == GLFW_PRESS)
    {
        Keys[key] = true;
    }
    if (action == GLFW_RELEASE)
    {
        Keys[key] = false;
    }
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_REPEAT || action == GLFW_PRESS)
    {
        MouseButtons[button] = true;
    }
    if (action == GLFW_RELEASE)
    {
        MouseButtons[button] = false;
    }
}

void Input::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (!JustStartedCursor)
    {
        MouseDelta = glm::vec2(xpos, ypos) - LastCursorPosition;
    }
    else
    {
        JustStartedCursor = false;
    }

    LastCursorPosition = glm::vec2(xpos, ypos);
    LastCursorPositionInputTime = glfwGetTime();
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
}

glm::vec2 Input::GetMouseDelta()
{
    bool MouseDeltaValid = fabs(LastCursorPositionInputTime - glfwGetTime()) < 0.001f;
    return (MouseDeltaValid)? MouseDelta : glm::vec2(0,0);
}
