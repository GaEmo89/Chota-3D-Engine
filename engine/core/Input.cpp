#include "Input.h"

GLFWwindow* Input::s_Window = nullptr;
double Input::s_MouseX = 0.0;
double Input::s_MouseY = 0.0;

void Input::Init(GLFWwindow* window)
{
    s_Window = window;
}

void Input::Update()
{
    glfwGetCursorPos(s_Window, &s_MouseX, &s_MouseY);
}

bool Input::KeyDown(int key)
{
    return glfwGetKey(s_Window, key) == GLFW_PRESS;
}

bool Input::MouseButtonDown(int button)
{
    return glfwGetMouseButton(s_Window, button) == GLFW_PRESS;
}

double Input::MouseX() { return s_MouseX; }
double Input::MouseY() { return s_MouseY; }
