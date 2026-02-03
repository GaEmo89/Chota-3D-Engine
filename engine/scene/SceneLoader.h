#pragma once
#include <string>

class Registry;
class RenderSystem;

class SceneLoader
{
public:
    static bool Load(const std::string& scenePath, Registry& reg, RenderSystem& renderer);
};
