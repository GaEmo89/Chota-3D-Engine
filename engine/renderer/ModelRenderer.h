#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "Model.h"

class Shader;
class Texture;

class ModelRenderer
{
public:
    ModelRenderer(const std::string& objPath, const std::string& fallbackTexture);
    ~ModelRenderer();

    bool IsValid() const;

    void Draw(const glm::mat4& modelMatrix,
              const glm::mat4& view,
              const glm::mat4& projection,
              const glm::vec3& cameraPos);

private:
    Model m_Model;
    Shader* m_Shader = nullptr;

    std::unordered_map<std::string, Texture*> m_TexturesByMaterial;
    Texture* m_Fallback = nullptr;

    bool m_Loaded = false;

private:
    Texture* GetTextureForMaterial(const std::string& mtlName) const;
};
