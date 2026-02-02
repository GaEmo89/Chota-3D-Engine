#pragma once
#include <string>
#include <unordered_map>
#include "Material.h"

class MTLLoader
{
public:
    // returns map: materialName -> Material
    static bool Load(const std::string& mtlPath,
                     std::unordered_map<std::string, Material>& outMats);
};
