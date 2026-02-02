#include "MTLLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

static std::string Trim(const std::string& s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

bool MTLLoader::Load(const std::string& mtlPath,
                     std::unordered_map<std::string, Material>& outMats)
{
    outMats.clear();

    std::ifstream file(mtlPath);
    if (!file.is_open())
    {
        std::cerr << "[MTL] Cannot open: " << mtlPath << "\n";
        return false;
    }

    Material current{};
    bool hasCurrent = false;

    std::string line;
    while (std::getline(file, line))
    {
        line = Trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "newmtl")
        {
            if (hasCurrent && !current.Name.empty())
                outMats[current.Name] = current;

            current = Material{};
            ss >> current.Name;
            hasCurrent = true;
        }
        else if (cmd == "map_Kd")
        {
            // rest of line can be path with spaces sometimes; we’ll take remaining
            std::string rest;
            std::getline(ss, rest);
            rest = Trim(rest);
            current.DiffuseMapPath = rest;
        }
        else
        {
            // ignore: Ka, Kd, Ks, Ns, d, illum, etc for now
        }
    }

    if (hasCurrent && !current.Name.empty())
        outMats[current.Name] = current;

    std::cout << "[MTL] Loaded materials: " << outMats.size() << " from " << mtlPath << "\n";
    return !outMats.empty();
}
