#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
{
    std::string vsrc = LoadFile(vertPath);
    std::string fsrc = LoadFile(fragPath);

    GLuint vs = Compile(GL_VERTEX_SHADER, vsrc.c_str());
    GLuint fs = Compile(GL_FRAGMENT_SHADER, fsrc.c_str());

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, fs);
    glLinkProgram(m_ID);

    GLint ok = 0;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetProgramInfoLog(m_ID, 1024, nullptr, log);
        std::cerr << "[SHADER LINK ERROR]\n" << log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    if (m_ID) glDeleteProgram(m_ID);
}

void Shader::Bind() const { glUseProgram(m_ID); }
void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetMat4(const char* name, const glm::mat4& m)
{
    GLint loc = glGetUniformLocation(m_ID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetVec3(const char* name, const glm::vec3& v)
{
    GLint loc = glGetUniformLocation(m_ID, name);
    glUniform3fv(loc, 1, &v[0]);
}

void Shader::SetInt(const char* name, int value)
{
    GLint loc = glGetUniformLocation(m_ID, name);
    glUniform1i(loc, value);
}

std::string Shader::LoadFile(const std::string& path)
{
    std::ifstream file(path, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "[SHADER ERROR] Cannot open file: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint Shader::Compile(GLenum type, const char* src)
{
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint ok = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetShaderInfoLog(id, 1024, nullptr, log);
        std::cerr << "[SHADER COMPILE ERROR]\n" << log << std::endl;
    }
    return id;
}
