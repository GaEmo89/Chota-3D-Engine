#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
};

class Mesh
{
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw() const;
    bool IsValid() const { return m_VAO != 0 && m_IndexCount > 0; }

private:
    void Destroy();

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;
    int m_IndexCount = 0;
};
