#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    m_IndexCount = (int)indices.size();
    if (vertices.empty() || indices.empty()) return;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // NOTE: Do not unbind GL_ELEMENT_ARRAY_BUFFER while VAO is bound; we already unbound VAO.
}

Mesh::~Mesh()
{
    Destroy();
}

Mesh::Mesh(Mesh&& other) noexcept
{
    m_VAO = other.m_VAO; other.m_VAO = 0;
    m_VBO = other.m_VBO; other.m_VBO = 0;
    m_EBO = other.m_EBO; other.m_EBO = 0;
    m_IndexCount = other.m_IndexCount; other.m_IndexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this == &other) return *this;
    Destroy();
    m_VAO = other.m_VAO; other.m_VAO = 0;
    m_VBO = other.m_VBO; other.m_VBO = 0;
    m_EBO = other.m_EBO; other.m_EBO = 0;
    m_IndexCount = other.m_IndexCount; other.m_IndexCount = 0;
    return *this;
}

void Mesh::Destroy()
{
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);
    m_VAO = m_VBO = m_EBO = 0;
    m_IndexCount = 0;
}

void Mesh::Draw() const
{
    if (!IsValid()) return;
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
