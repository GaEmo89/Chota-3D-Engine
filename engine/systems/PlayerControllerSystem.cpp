#include "PlayerControllerSystem.h"

#include "ecs/Registry.h"
#include "components/Transform.h"
#include "components/PlayerControllerComponent.h"
#include "core/Input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

static glm::vec3 ForwardFromYawPitch(float yawDeg, float pitchDeg)
{
    glm::vec3 front;
    front.x = cos(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
    front.y = sin(glm::radians(pitchDeg));
    front.z = sin(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
    return glm::normalize(front);
}

void PlayerControllerSystem::Update(Registry& reg, float dt)
{
    // Controla TODAS las entidades con PlayerControllerComponent
    for (Entity e : reg.View<PlayerControllerComponent>())
    {
        auto& pc = reg.Get<PlayerControllerComponent>(e);
        auto& tr = reg.Get<Transform>(e);

        // Mouse look (yaw/pitch)
        static bool firstMouse = true;
        static double lastX = 0.0, lastY = 0.0;

        double mx = Input::MouseX();
        double my = Input::MouseY();

        if (firstMouse)
        {
            lastX = mx; lastY = my;
            firstMouse = false;
        }

        float dx = (float)(mx - lastX) * pc.MouseSensitivity;
        float dy = (float)(lastY - my) * pc.MouseSensitivity;

        lastX = mx;
        lastY = my;

        pc.Yaw += dx;
        pc.Pitch += dy;

        if (pc.Pitch > 89.0f) pc.Pitch = 89.0f;
        if (pc.Pitch < -89.0f) pc.Pitch = -89.0f;

        // Movimiento SOLO en XZ (pitch=0) para que no “vuele”
        glm::vec3 forward = ForwardFromYawPitch(pc.Yaw, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

        glm::vec3 move(0.0f);
        if (Input::KeyDown(GLFW_KEY_W)) move += forward;
        if (Input::KeyDown(GLFW_KEY_S)) move -= forward;
        if (Input::KeyDown(GLFW_KEY_D)) move += right;
        if (Input::KeyDown(GLFW_KEY_A)) move -= right;

        if (glm::length(move) > 0.0f)
            move = glm::normalize(move);

        tr.Position += move * pc.MoveSpeed * dt;

        // opcional: si el player tuviera modelo visible, lo giramos con yaw
        tr.RotationEulerDeg.y = pc.Yaw;
    }
}
