#pragma once
#include <string>
#include "Model.h"

class OBJLoader
{
public:
    static bool LoadModel(const std::string& objPath, Model& outModel);
};
