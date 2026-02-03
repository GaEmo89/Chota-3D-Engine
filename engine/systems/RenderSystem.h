#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Registry;
class Camera;
class ModelRenderer;

class AssetManager
{
public:
    std::shared_ptr<ModelRenderer> LoadModel(const std::string& objPath,
                                             const std::string& fallbackTexture);

private:
    std::unordered_map<std::string, std::shared_ptr<ModelRenderer>> m_ModelCache;
};

class RenderSystem
{
public:
    void Render(Registry& reg,
                const Camera& camera,
                float timeSeconds);

    AssetManager& Assets() { return m_Assets; }

private:
    AssetManager m_Assets;
};
