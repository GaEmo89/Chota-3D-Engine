#pragma once
#include <GLFW/glfw3.h>

class Input
{
public:
    static void Init(GLFWwindow* window);
    static void Update();

    static bool KeyDown(int key);
    static bool MouseButtonDown(int button);

    static double MouseX();
    static double MouseY();

private:
    static GLFWwindow* s_Window;
    static double s_MouseX;
    static double s_MouseY;
};
