#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetMat4(const char* name, const glm::mat4& m);
    void SetVec3(const char* name, const glm::vec3& v);
    void SetInt (const char* name, int value);

private:
    GLuint m_ID = 0;

    std::string LoadFile(const std::string& path);
    GLuint Compile(GLenum type, const char* src);
};
