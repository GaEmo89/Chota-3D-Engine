#pragma once
#include <string>

class Texture
{
public:
    explicit Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;

private:
    unsigned int m_ID = 0;
};
