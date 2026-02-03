#pragma once

struct PlayerControllerComponent
{
    float MoveSpeed = 6.0f;
    float MouseSensitivity = 0.12f;

    float Yaw = -90.0f;
    float Pitch = 0.0f;

    // Jump / gravity
    float Gravity = -18.0f;     // feel tipo FPS
    float JumpSpeed = 7.5f;
    float GroundY = 0.0f;       // piso plano y=0
    float PlayerHeight = 1.0f;  // la cámara queda a 1 unidad del suelo
};
