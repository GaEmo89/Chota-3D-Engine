#include "ModelRenderer.h"
#include "OBJLoader.h"
#include "Shader.h"
#include "Texture.h"

#include <iostream>

static std::string JoinPath(const std::string& dir, const std::string& rel)
{
    if (rel.empty()) return "";
    if (rel.find(':') != std::string::npos) return rel;
    if (!dir.empty() && (rel[0] == '/' || rel[0] == '\\')) return rel;
    return dir + rel;
}

ModelRenderer::ModelRenderer(const std::string& objPath, const std::string& fallbackTexture)
{
    m_Loaded = OBJLoader::LoadModel(objPath, m_Model);

    m_Shader = new Shader("shaders/textured.vert", "shaders/textured.frag");
    m_Fallback = new Texture(fallbackTexture);

    m_Shader->Bind();
    m_Shader->SetInt("u_Texture", 0);
    m_Shader->Unbind();

    // Create textures for each material that has map_Kd
    for (auto& kv : m_Model.materials)
    {
        const std::string& name = kv.first;
        const Material& mat = kv.second;

        if (mat.DiffuseMapPath.empty())
            continue;

        std::string full = JoinPath(m_Model.directory, mat.DiffuseMapPath);
        Texture* tex = new Texture(full);
        m_TexturesByMaterial[name] = tex;

        std::cout << "[MAT] " << name << " map_Kd=" << full << "\n";
    }
}

ModelRenderer::~ModelRenderer()
{
    delete m_Shader;

    for (auto& kv : m_TexturesByMaterial)
        delete kv.second;

    delete m_Fallback;
}

bool ModelRenderer::IsValid() const
{
    return m_Loaded && m_Shader && m_Fallback && !m_Model.submeshes.empty();
}

Texture* ModelRenderer::GetTextureForMaterial(const std::string& mtlName) const
{
    auto it = m_TexturesByMaterial.find(mtlName);
    if (it != m_TexturesByMaterial.end())
        return it->second;
    return m_Fallback;
}

void ModelRenderer::Draw(const glm::mat4& modelMatrix,
                         const glm::mat4& view,
                         const glm::mat4& projection,
                         const glm::vec3& cameraPos)
{
    if (!IsValid()) return;

    m_Shader->Bind();
    m_Shader->SetMat4("u_Model", modelMatrix);
    m_Shader->SetMat4("u_View", view);
    m_Shader->SetMat4("u_Projection", projection);
    m_Shader->SetVec3("u_LightDir", glm::vec3(-1.0f, -1.0f, -1.0f));
    m_Shader->SetVec3("u_ViewPos", cameraPos);

    for (const auto& sm : m_Model.submeshes)
    {
        Texture* tex = GetTextureForMaterial(sm.materialName);
        tex->Bind(0);
        sm.mesh.Draw();
    }

    m_Shader->Unbind();
}
