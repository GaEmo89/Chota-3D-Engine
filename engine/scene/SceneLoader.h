#pragma once
#include <string>

class Registry;
class RenderSystem;

class SceneLoader
{
public:
    // Lee un .scene y spawnea entidades en el Registry
    static bool Load(const std::string& scenePath, Registry& reg, RenderSystem& renderer);
};
