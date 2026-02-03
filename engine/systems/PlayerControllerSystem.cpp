#include "PlayerControllerSystem.h"

#include "ecs/Registry.h"
#include "components/Transform.h"
#include "components/PlayerControllerComponent.h"
#include "components/RigidBodyComponent.h"
#include "core/Input.h"

#include <glm/glm.hpp>
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
    for (Entity e : reg.View<PlayerControllerComponent>())
    {
        auto& pc = reg.Get<PlayerControllerComponent>(e);
        auto& tr = reg.Get<Transform>(e);

        // Ensure rigidbody exists
        if (!reg.Has<RigidBodyComponent>(e))
            reg.Add<RigidBodyComponent>(e);

        auto& rb = reg.Get<RigidBodyComponent>(e);

        // =========================
        // Mouse look (yaw/pitch)
        // =========================
        // Usamos deltas de Input para que sea suave y sin “firstMouse” raro
        pc.Yaw   += (float)Input::MouseDeltaX() * pc.MouseSensitivity;
        pc.Pitch += (float)(-Input::MouseDeltaY()) * pc.MouseSensitivity;

        if (pc.Pitch > 89.0f) pc.Pitch = 89.0f;
        if (pc.Pitch < -89.0f) pc.Pitch = -89.0f;

        // =========================
        // Horizontal movement (XZ)
        // =========================
        glm::vec3 forward = ForwardFromYawPitch(pc.Yaw, 0.0f); // pitch=0 => no vuelas
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

        glm::vec3 move(0.0f);
        if (Input::KeyDown(GLFW_KEY_W)) move += forward;
        if (Input::KeyDown(GLFW_KEY_S)) move -= forward;
        if (Input::KeyDown(GLFW_KEY_D)) move += right;
        if (Input::KeyDown(GLFW_KEY_A)) move -= right;

        if (glm::length(move) > 0.0f)
            move = glm::normalize(move);

        tr.Position += move * pc.MoveSpeed * dt;

        // =========================
        // Ground check (simple plane)
        // feet at y = position.y - PlayerHeight
        // =========================
        float feetY = tr.Position.y - pc.PlayerHeight;

        if (feetY <= pc.GroundY)
        {
            tr.Position.y = pc.GroundY + pc.PlayerHeight;
            rb.Velocity.y = 0.0f;
            rb.Grounded = true;
        }
        else
        {
            rb.Grounded = false;
        }

        // =========================
        // Jump
        // =========================
        if (rb.Grounded && Input::KeyPressed(GLFW_KEY_SPACE))
        {
            rb.Velocity.y = pc.JumpSpeed;
            rb.Grounded = false;
        }

        // =========================
        // Gravity & integrate Y
        // =========================
        rb.Velocity.y += pc.Gravity * dt;
        tr.Position.y += rb.Velocity.y * dt;

        // Optional: align “body” yaw
        tr.RotationEulerDeg.y = pc.Yaw;
    }
}
