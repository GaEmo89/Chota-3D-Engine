#include "Input.h"
#include <cstring>

GLFWwindow* Input::s_Window = nullptr;

bool Input::s_Keys[GLFW_KEY_LAST + 1] = {};
bool Input::s_KeysPrev[GLFW_KEY_LAST + 1] = {};

double Input::s_MouseX = 0.0;
double Input::s_MouseY = 0.0;
double Input::s_PrevMouseX = 0.0;
double Input::s_PrevMouseY = 0.0;
double Input::s_DeltaX = 0.0;
double Input::s_DeltaY = 0.0;

void Input::Init(GLFWwindow* window)
{
    s_Window = window;
    std::memset(s_Keys, 0, sizeof(s_Keys));
    std::memset(s_KeysPrev, 0, sizeof(s_KeysPrev));

    glfwGetCursorPos(s_Window, &s_MouseX, &s_MouseY);
    s_PrevMouseX = s_MouseX;
    s_PrevMouseY = s_MouseY;
    s_DeltaX = s_DeltaY = 0.0;
}

void Input::Update()
{
    // Copy current -> prev
    std::memcpy(s_KeysPrev, s_Keys, sizeof(s_Keys));

    // Poll keys
    for (int k = 0; k <= GLFW_KEY_LAST; k++)
        s_Keys[k] = (glfwGetKey(s_Window, k) == GLFW_PRESS);

    // Mouse
    s_PrevMouseX = s_MouseX;
    s_PrevMouseY = s_MouseY;

    glfwGetCursorPos(s_Window, &s_MouseX, &s_MouseY);

    s_DeltaX = s_MouseX - s_PrevMouseX;
    s_DeltaY = s_MouseY - s_PrevMouseY;
}

bool Input::KeyDown(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return s_Keys[key];
}

bool Input::KeyPressed(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return s_Keys[key] && !s_KeysPrev[key];
}

bool Input::KeyReleased(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return !s_Keys[key] && s_KeysPrev[key];
}

double Input::MouseX() { return s_MouseX; }
double Input::MouseY() { return s_MouseY; }

double Input::MouseDeltaX() { return s_DeltaX; }
double Input::MouseDeltaY() { return s_DeltaY; }
