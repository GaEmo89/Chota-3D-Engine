#pragma once
#include <glm/glm.hpp>

struct RigidBodyComponent
{
    glm::vec3 Velocity{ 0.0f };
    bool Grounded = false;
};
