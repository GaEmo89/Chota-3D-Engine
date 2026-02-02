#pragma once
#include <vector>
#include <string>
#include "Mesh.h"
#include "Material.h"

struct SubMesh
{
    Mesh mesh;
    std::string materialName;
};

class Model
{
public:
    std::vector<SubMesh> submeshes;
    std::unordered_map<std::string, Material> materials;
    std::string directory; // folder of obj (for relative paths)
};
