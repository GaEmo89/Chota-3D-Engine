#include "OBJLoader.h"
#include "MTLLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

struct ObjIndex { int v=0, vt=0, vn=0; };

static std::string DirOf(const std::string& path)
{
    size_t p = path.find_last_of("/\\");
    if (p == std::string::npos) return "";
    return path.substr(0, p + 1);
}

static bool ParseFaceToken(const std::string& tok, ObjIndex& out)
{
    out = {};
    size_t first = tok.find('/');
    if (first == std::string::npos)
    {
        out.v = std::stoi(tok);
        return true;
    }

    std::string a = tok.substr(0, first);
    out.v = a.empty() ? 0 : std::stoi(a);

    size_t second = tok.find('/', first + 1);
    if (second == std::string::npos)
    {
        std::string b = tok.substr(first + 1);
        out.vt = b.empty() ? 0 : std::stoi(b);
        return true;
    }

    std::string b = tok.substr(first + 1, second - (first + 1));
    std::string c = tok.substr(second + 1);

    out.vt = b.empty() ? 0 : std::stoi(b);
    out.vn = c.empty() ? 0 : std::stoi(c);
    return true;
}

static int FixIndex(int idx, int count)
{
    if (idx > 0) return idx - 1;
    if (idx < 0) return count + idx;
    return -1;
}

struct Key
{
    int v, vt, vn;
    bool operator==(const Key& o) const { return v==o.v && vt==o.vt && vn==o.vn; }
};
struct KeyHash
{
    size_t operator()(const Key& k) const noexcept
    {
        return (size_t)k.v * 73856093u ^ (size_t)k.vt * 19349663u ^ (size_t)k.vn * 83492791u;
    }
};

struct BuildMesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<Key, unsigned int, KeyHash> map;
};

static void PushTri(BuildMesh& bm,
                    const ObjIndex& a, const ObjIndex& b, const ObjIndex& c,
                    const std::vector<glm::vec3>& pos,
                    const std::vector<glm::vec2>& uv,
                    const std::vector<glm::vec3>& nor)
{
    ObjIndex tri[3] = { a,b,c };
    for (int t=0;t<3;t++)
    {
        int pv = FixIndex(tri[t].v,  (int)pos.size());
        int pt = FixIndex(tri[t].vt, (int)uv.size());
        int pn = FixIndex(tri[t].vn, (int)nor.size());

        Key key{ pv, pt, pn };
        auto it = bm.map.find(key);
        if (it != bm.map.end())
        {
            bm.indices.push_back(it->second);
            continue;
        }

        Vertex v{};
        v.Position = (pv>=0) ? pos[pv] : glm::vec3(0);
        v.UV       = (pt>=0) ? uv[pt]  : glm::vec2(0);
        v.Normal   = (pn>=0) ? nor[pn] : glm::vec3(0,0,1);

        unsigned int newIndex = (unsigned int)bm.vertices.size();
        bm.vertices.push_back(v);
        bm.indices.push_back(newIndex);
        bm.map.emplace(key, newIndex);
    }
}

bool OBJLoader::LoadModel(const std::string& objPath, Model& outModel)
{
    outModel = Model{};
    outModel.directory = DirOf(objPath);

    std::ifstream file(objPath);
    if (!file.is_open())
    {
        std::cerr << "[OBJ] Cannot open: " << objPath << "\n";
        return false;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    std::string activeMtl = "default";
    std::unordered_map<std::string, BuildMesh> builders;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0]=='#') continue;

        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "mtllib")
        {
            std::string mtlFile;
            ss >> mtlFile;
            std::string mtlPath = outModel.directory + mtlFile;
            MTLLoader::Load(mtlPath, outModel.materials);
        }
        else if (type == "usemtl")
        {
            ss >> activeMtl;
            if (activeMtl.empty()) activeMtl = "default";
        }
        else if (type == "v")
        {
            glm::vec3 p; ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        }
        else if (type == "vt")
        {
            glm::vec2 uv; ss >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (type == "vn")
        {
            glm::vec3 n; ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (type == "f")
        {
            std::vector<ObjIndex> face;
            face.reserve(8);

            std::string tok;
            while (ss >> tok)
            {
                ObjIndex idx;
                if (ParseFaceToken(tok, idx)) face.push_back(idx);
            }
            if (face.size() < 3) continue;

            BuildMesh& bm = builders[activeMtl];

            // triangulate fan
            for (size_t i=1;i+1<face.size();i++)
            {
                PushTri(bm, face[0], face[i], face[i+1], positions, uvs, normals);
            }
        }
    }

    // Build submeshes
    for (auto& kv : builders)
    {
        const std::string& mtlName = kv.first;
        BuildMesh& bm = kv.second;

        if (bm.vertices.empty() || bm.indices.empty()) continue;

        SubMesh sm;
        sm.materialName = mtlName;
        sm.mesh = Mesh(bm.vertices, bm.indices);
        outModel.submeshes.push_back(std::move(sm));
    }

    std::cout << "[OBJ] Submeshes: " << outModel.submeshes.size()
              << " from " << objPath << "\n";
    return !outModel.submeshes.empty();
}
