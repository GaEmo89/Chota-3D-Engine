#pragma once
#include <GLFW/glfw3.h>

class Input
{
public:
    static void Init(GLFWwindow* window);
    static void Update();

    static bool KeyDown(int key);      // held
    static bool KeyPressed(int key);   // just pressed this frame
    static bool KeyReleased(int key);  // just released this frame

    static double MouseX();
    static double MouseY();
    static double MouseDeltaX();
    static double MouseDeltaY();

private:
    static GLFWwindow* s_Window;

    static bool s_Keys[GLFW_KEY_LAST + 1];
    static bool s_KeysPrev[GLFW_KEY_LAST + 1];

    static double s_MouseX;
    static double s_MouseY;
    static double s_PrevMouseX;
    static double s_PrevMouseY;
    static double s_DeltaX;
    static double s_DeltaY;
};
