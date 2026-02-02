#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovDeg, float aspect, float nearPlane, float farPlane)
    : m_FOV(fovDeg), m_Aspect(aspect), m_Near(nearPlane), m_Far(farPlane)
{
    UpdateVectors();
}

void Camera::SetYawPitch(float yawDeg, float pitchDeg)
{
    m_Yaw = yawDeg;
    m_Pitch = pitchDeg;
    UpdateVectors();
}

void Camera::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0, 1, 0)));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}

glm::mat4 Camera::GetView() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjection() const
{
    return glm::perspective(glm::radians(m_FOV), m_Aspect, m_Near, m_Far);
}
