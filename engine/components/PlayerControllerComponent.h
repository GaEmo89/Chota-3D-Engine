#pragma once
#include <glm/glm.hpp>

struct PlayerControllerComponent
{
    float MoveSpeed = 5.0f;
    float MouseSensitivity = 0.12f;

    float Yaw = -90.0f;
    float Pitch = 0.0f;
};
