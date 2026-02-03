#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/Input.h"
#include "math/Camera.h"

#include "ecs/Registry.h"
#include "systems/RenderSystem.h"
#include "scene/SceneLoader.h"

#include "components/Transform.h"
#include "components/PlayerControllerComponent.h"
#include "components/RigidBodyComponent.h"
#include "systems/PlayerControllerSystem.h"

static void FramebufferSizeCallback(GLFWwindow*, int w, int h)
{
    glViewport(0, 0, w, h);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "CHOTANENGINE", nullptr, nullptr);
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    Input::Init(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.05f, 0.05f, 0.10f, 1.0f);

    Camera camera(75.0f, 1280.0f / 720.0f, 0.1f, 200.0f);

    // ECS + systems
    Registry reg;
    RenderSystem renderSystem;
    PlayerControllerSystem playerSystem;

    // Load scene (props)
    SceneLoader::Load("assets/scenes/test.scene", reg, renderSystem);

    // Create Player
    Entity player = reg.Create();

    auto& pt = reg.Add<Transform>(player);
    pt.Position = { 0.0f, 1.0f, 3.0f };
    pt.Scale = { 1.0f, 1.0f, 1.0f };

    auto& pc = reg.Add<PlayerControllerComponent>(player);
    pc.MoveSpeed = 6.0f;
    pc.MouseSensitivity = 0.12f;
    pc.Gravity = -18.0f;
    pc.JumpSpeed = 7.5f;
    pc.GroundY = 0.0f;
    pc.PlayerHeight = 1.0f;

    auto& rb = reg.Add<RigidBodyComponent>(player);
    rb.Velocity = { 0.0f, 0.0f, 0.0f };
    rb.Grounded = false;

    float last = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float now = (float)glfwGetTime();
        float dt = now - last;
        last = now;

        glfwPollEvents();
        Input::Update();

        if (Input::KeyPressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);

        // Update gameplay
        playerSystem.Update(reg, dt);

        // Camera = player pos + view angles
        auto& pctr = reg.Get<Transform>(player);
        auto& pcc  = reg.Get<PlayerControllerComponent>(player);

        camera.SetPosition(pctr.Position);
        camera.SetYawPitch(pcc.Yaw, pcc.Pitch);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderSystem.Render(reg, camera, now);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
