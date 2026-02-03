#include "RenderSystem.h"

#include "ecs/Registry.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"

#include "math/Camera.h"
#include "renderer/ModelRenderer.h"

#include <glm/glm.hpp>

std::shared_ptr<ModelRenderer> AssetManager::LoadModel(const std::string& objPath,
                                                       const std::string& fallbackTexture)
{
    std::string key = objPath + "|" + fallbackTexture;

    auto it = m_ModelCache.find(key);
    if (it != m_ModelCache.end())
        return it->second;

    auto model = std::make_shared<ModelRenderer>(objPath, fallbackTexture);
    m_ModelCache[key] = model;
    return model;
}

void RenderSystem::Render(Registry& reg,
                          const Camera& camera,
                          float /*timeSeconds*/)
{
    auto entities = reg.View<MeshRendererComponent>();

    for (Entity e : entities)
    {
        auto& mr = reg.Get<MeshRendererComponent>(e);
        if (!mr.Model || !mr.Model->IsValid())
            continue;

        glm::mat4 modelMat(1.0f);
        if (reg.Has<Transform>(e))
            modelMat = reg.Get<Transform>(e).ToMatrix();

        mr.Model->Draw(
            modelMat,
            camera.GetView(),
            camera.GetProjection(),
            camera.GetPosition()
        );
    }
}
