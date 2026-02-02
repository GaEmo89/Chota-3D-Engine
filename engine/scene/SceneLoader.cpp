#include "SceneLoader.h"

#include "ecs/Registry.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "systems/RenderSystem.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

static std::string Trim(const std::string& s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

struct PendingEntity
{
    std::string name;

    glm::vec3 pos{0.0f};
    glm::vec3 rot{0.0f};
    glm::vec3 scale{1.0f};

    std::string modelPath;
    std::string fallbackTex;

    bool hasEntity = false;
};

static void SpawnPending(PendingEntity& p, Registry& reg, RenderSystem& renderer)
{
    if (!p.hasEntity) return;

    // Must have model + fallback
    if (p.modelPath.empty() || p.fallbackTex.empty())
    {
        std::cout << "[SCENE] Skip entity '" << p.name
                  << "' (missing model/fallback)\n";
        p = PendingEntity{};
        return;
    }

    Entity e = reg.Create();

    auto& t = reg.Add<Transform>(e);
    t.Position = p.pos;
    t.RotationEulerDeg = p.rot;
    t.Scale = p.scale;

    auto& mr = reg.Add<MeshRendererComponent>(e);
    mr.Model = renderer.Assets().LoadModel(p.modelPath, p.fallbackTex);

    std::cout << "[SCENE] Spawned '" << p.name << "' model=" << p.modelPath << "\n";

    p = PendingEntity{};
}

bool SceneLoader::Load(const std::string& scenePath, Registry& reg, RenderSystem& renderer)
{
    std::ifstream file(scenePath);
    if (!file.is_open())
    {
        std::cout << "[SCENE] Cannot open: " << scenePath << "\n";
        return false;
    }

    PendingEntity pending{};
    std::string line;

    while (std::getline(file, line))
    {
        line = Trim(line);
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "entity")
        {
            // If previous entity wasn’t ended, spawn it
            SpawnPending(pending, reg, renderer);

            pending = PendingEntity{};
            pending.hasEntity = true;
            ss >> pending.name;
            if (pending.name.empty()) pending.name = "unnamed";
        }
        else if (cmd == "pos")
        {
            ss >> pending.pos.x >> pending.pos.y >> pending.pos.z;
        }
        else if (cmd == "rot")
        {
            ss >> pending.rot.x >> pending.rot.y >> pending.rot.z;
        }
        else if (cmd == "scale")
        {
            ss >> pending.scale.x >> pending.scale.y >> pending.scale.z;
        }
        else if (cmd == "model")
        {
            std::string rest;
            std::getline(ss, rest);
            pending.modelPath = Trim(rest);
        }
        else if (cmd == "fallback")
        {
            std::string rest;
            std::getline(ss, rest);
            pending.fallbackTex = Trim(rest);
        }
        else if (cmd == "end")
        {
            SpawnPending(pending, reg, renderer);
        }
        else
        {
            std::cout << "[SCENE] Unknown command: " << cmd << "\n";
        }
    }

    // If file ended without "end"
    SpawnPending(pending, reg, renderer);

    return true;
}
