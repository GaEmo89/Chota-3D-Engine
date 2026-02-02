#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::vec3 Position{ 0.0f };
    glm::vec3 RotationEulerDeg{ 0.0f }; // pitch(x), yaw(y), roll(z)
    glm::vec3 Scale{ 1.0f };

    glm::mat4 ToMatrix() const
    {
        glm::mat4 m(1.0f);
        m = glm::translate(m, Position);

        m = glm::rotate(m, glm::radians(RotationEulerDeg.x), glm::vec3(1,0,0));
        m = glm::rotate(m, glm::radians(RotationEulerDeg.y), glm::vec3(0,1,0));
        m = glm::rotate(m, glm::radians(RotationEulerDeg.z), glm::vec3(0,0,1));

        m = glm::scale(m, Scale);
        return m;
    }
};
