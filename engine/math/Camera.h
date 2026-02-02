#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(float fovDeg, float aspect, float nearPlane, float farPlane);

    void SetPosition(const glm::vec3& p) { m_Position = p; }
    void SetYawPitch(float yawDeg, float pitchDeg);

    glm::mat4 GetView() const;
    glm::mat4 GetProjection() const;

    glm::vec3 GetPosition() const { return m_Position; }

private:
    void UpdateVectors();

private:
    glm::vec3 m_Position{ 0.0f, 0.0f, 3.0f };
    glm::vec3 m_Front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };

    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;

    float m_FOV;
    float m_Aspect;
    float m_Near;
    float m_Far;
};
