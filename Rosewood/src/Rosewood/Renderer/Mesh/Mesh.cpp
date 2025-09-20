#include "rwpch.h"
#include "Mesh.h"
#include "Rosewood/Utils/Conversions.h"

#include <unordered_map>

namespace Rosewood
{
    Mesh::Mesh(const MeshData& data):
        m_MeshData(data)
    {}

    Mesh::Mesh(MeshData&& data):
        m_MeshData(data)
    {}

    std::vector<Ref<VertexArray>> Mesh::MakeVertexArrays()
    {
        struct Vertex
        {
            glm::vec3 Position = glm::vec3{};
            glm::vec3 Normal = glm::vec3{};
            glm::vec2 TexCoords = glm::vec2{};
            uint32_t Color = 0;
        };

        Rosewood::BufferLayout layout = {
            { Rosewood::ShaderDataType::Float3,				"a_Position"		},
            { Rosewood::ShaderDataType::Float3,				"a_Normal"			},
            { Rosewood::ShaderDataType::Float2,			    "a_UV"				},
            { Rosewood::ShaderDataType::Packed_2ZYX_UInt,	"a_Color",		true},
        };

        std::vector<Ref<VertexArray>> VAOs;

        for (MeshData::SurfaceData& surface : m_MeshData.Surfaces)
        {
            std::vector<Vertex> interleavedVerticesData(surface.Positions.size());

            uint32_t i = 0;
            for (Vertex& vertex : interleavedVerticesData)
            {
                vertex.Position = surface.Positions[i];

                vertex.Normal = surface.Normals[i];

                //vertex.TexCoords[0] = (uint16_t)std::round(65535.f * surface.TexCoords[i].x);
                //vertex.TexCoords[1] = (uint16_t)std::round(65535.f * surface.TexCoords[i].y);
                vertex.TexCoords[0] = surface.TexCoords[i].x;
                vertex.TexCoords[1] = surface.TexCoords[i].y;

                uint32_t r = (uint32_t)std::round(1023.f * .0f);
                uint32_t g = (uint32_t)std::round(1023.f * .0f);
                uint32_t b = (uint32_t)std::round(1023.f * .0f);
                uint32_t a = (uint32_t)(3.f * 1.f);
                vertex.Color = (a << 30) | (b << 20) | (g << 10) | (r << 0);

                i++;
            }


            Ref<VertexArray> VAO = Rosewood::VertexArray::Create();
            VAO->Bind();

            Rosewood::Ref<Rosewood::VertexBuffer>   vb  = Rosewood::VertexBuffer::Create((BYTE*)interleavedVerticesData.data(), Utils::SizeToUint32(interleavedVerticesData.size()) * sizeof(Vertex), layout);
            Ref<Rosewood::IndexBuffer>              ib  = IndexBuffer::Create(surface.Indices.data(), Utils::SizeToUint32(surface.Indices.size()));

            VAO->AddVertexBuffer(vb);
            VAO->SetIndexBuffer(ib);

            VAOs.push_back(VAO);
        }

        return VAOs;
    }

    std::vector<MeshData::SurfaceData>& Mesh::GetSurfaces()
    {
        return m_MeshData.Surfaces;
    }

    ModelImport::ImportedModel ModelImport::Import(const std::string& filepath, FileFormat fileFormat)
    {
        ModelImport::ImportedModel model = ModelImport::ImportedModel{};
        switch (fileFormat)
        {
        case Rosewood::ModelImport::FileFormat::OBJ :
            ModelImport::ImportFromOBJ(filepath, model);
            return model;
        }

        RW_CORE_ASSERT(false, " Model file format not supported! ")
        return model;
    }

    void ModelImport::ImportFromOBJ(const std::string& filepath, ModelImport::ImportedModel& out_Model)
    {
        struct TempSurfaceData
        {
            std::vector<glm::ivec3> Indices;
            std::vector<uint32_t> PositionsIndices;
            std::vector<uint32_t> NormalsIndices;
            std::vector<uint32_t> TexCoordsIndices;
            std::string MaterialSlotName = "";
        };

        struct TempMeshObjectData
        {
            std::vector<TempSurfaceData> Surfaces;

            std::vector<glm::vec3> Positions;
            std::vector<glm::vec3> Normals;
            std::vector<glm::vec2> TexCoords;
            std::vector<glm::vec3> Colors;

            std::string ObjectName = "";
        };

        std::vector<TempMeshObjectData> tempMeshObjects;
        bool inSurface = false;

        std::ifstream fileStream;
        fileStream.open(filepath);

        RW_CORE_ASSERT(fileStream.is_open(), "Unable to open file : '" + filepath + "'");

        std::string line;
        while (std::getline(fileStream, line)) {
            std::string lineHead;
            std::istringstream lineBodyStream(line);
            lineBodyStream >> lineHead;

            if (lineHead == "o") {
                tempMeshObjects.push_back(TempMeshObjectData());
                lineBodyStream >> tempMeshObjects.back().ObjectName;
            }

            else if (lineHead == "v") {
                std::string x, y, z;
                lineBodyStream >> x; lineBodyStream >> y; lineBodyStream >> z;
                tempMeshObjects.back().Positions.push_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));

                if (!(lineBodyStream.peek() == EOF))
                {
                    lineBodyStream >> x; lineBodyStream >> y; lineBodyStream >> z;
                    tempMeshObjects.back().Colors.push_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
                }
            }

            else if (lineHead == "vn") {
                std::string x, y, z;
                lineBodyStream >> x; lineBodyStream >> y; lineBodyStream >> z;
                tempMeshObjects.back().Normals.push_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
            }

            else if (lineHead == "vt") {
                std::string x, y;
                lineBodyStream >> x; lineBodyStream >> y;
                tempMeshObjects.back().TexCoords.push_back(glm::vec2(std::stof(x), std::stof(y)));
            }

            else if (lineHead == "usemtl")
            {
                tempMeshObjects.back().Surfaces.push_back(TempSurfaceData());
                lineBodyStream >> tempMeshObjects.back().Surfaces.back().MaterialSlotName;
                if (!inSurface)
                    inSurface = true;
            }

            else if (lineHead == "f") {
                if (!inSurface)
                {
                    tempMeshObjects.back().Surfaces.push_back(TempSurfaceData());
                    inSurface = true;
                }

                // only works correctly on trianglulated meshes
                std::string a, b, c;
                lineBodyStream >> a; lineBodyStream >> b; lineBodyStream >> c;

                std::string delimiter = "/";
                for (std::string str : {a, b, c}) {
                    uint32_t x = 0, y = 0, z = 0;

                    size_t pos = 0;
                    pos = str.find(delimiter);
                    x = std::stoi(str.substr(0, pos)) - 1;
                  
                    str.erase(0, pos + delimiter.length());
                    
                    pos = str.find(delimiter);
                    if (pos>0)
                        y = std::stoi(str.substr(0, pos)) - 1;
                    str.erase(0, pos + delimiter.length());

                    if (!str.empty())
                        z = std::stoi(str) - 1;

                    tempMeshObjects.back().Surfaces.back().Indices.push_back(glm::ivec3(x, y, z));
                }       
            }
        }

        fileStream.close();

        uint32_t positionIndexOffset = 0;
        uint32_t normalsIndexOffset = 0;
        uint32_t texCoordsIndexOffset = 0;

        for (TempMeshObjectData tempMeshObject : tempMeshObjects)
        {
            MeshData meshData = MeshData();
            meshData.ObjectName = tempMeshObject.ObjectName;

            for (TempSurfaceData tempSurface : tempMeshObject.Surfaces)
            {
                meshData.Surfaces.push_back(MeshData::SurfaceData());
                meshData.SlotNames.push_back(tempSurface.MaterialSlotName);

                std::vector<glm::vec3>& positions = meshData.Surfaces.back().Positions;
                std::vector<glm::vec3>& normals = meshData.Surfaces.back().Normals;
                std::vector<glm::vec2>& texCoords = meshData.Surfaces.back().TexCoords;
                std::vector<glm::vec3>& colors = meshData.Surfaces.back().Colors;

                std::vector<uint32_t>& indices = meshData.Surfaces.back().Indices;

                std::unordered_map<std::string, uint32_t> map{};
                map.reserve(tempSurface.Indices.size());

                uint32_t uniqueVertexCount = 0;
                uint32_t indicesCount = 0;
                for (glm::ivec3 indexTriplet : tempSurface.Indices)
                {
                    std::string key = std::to_string(indexTriplet.x) + "-" + std::to_string(indexTriplet.y) + "-" + std::to_string(indexTriplet.z);
                    if (map.count(key))
                    {
                        indicesCount++;
                        continue;
                    }

                    map.insert({ key, uniqueVertexCount });

                    indicesCount++;
                    uniqueVertexCount++;
                }

                positions.resize(uniqueVertexCount);
                if (tempMeshObject.Normals.size() > 0)
                    normals.resize(uniqueVertexCount);
                if (tempMeshObject.TexCoords.size() > 0)
                    texCoords.resize(uniqueVertexCount);
                if (tempMeshObject.Colors.size() > 0)
                    colors.resize(uniqueVertexCount);

                indices = std::vector<uint32_t>(indicesCount);

                map.clear();
                map.reserve(tempSurface.Indices.size());

                uniqueVertexCount = 0;
                indicesCount = 0;
                for (glm::ivec3 indexTriplet : tempSurface.Indices)
                {
                    std::string key = std::to_string(indexTriplet.x) + "-" + std::to_string(indexTriplet.y) + "-" + std::to_string(indexTriplet.z);
                    if (map.count(key))
                    {
                        indices[indicesCount] = map.at(key);
                        indicesCount++;
                        continue;
                    }

                    uint32_t posIndex = indexTriplet.x - positionIndexOffset;
                    uint32_t normIndex = indexTriplet.z - normalsIndexOffset;
                    uint32_t texIndex = indexTriplet.y - texCoordsIndexOffset;

                    positions[uniqueVertexCount] = tempMeshObject.Positions[posIndex];
                    if (tempMeshObject.Normals.size() > 0)
                        normals[uniqueVertexCount] = tempMeshObject.Normals[normIndex];
                    if (tempMeshObject.TexCoords.size() > 0)
                        texCoords[uniqueVertexCount] = tempMeshObject.TexCoords[texIndex];
                    if (tempMeshObject.Colors.size() > 0)
                        colors[uniqueVertexCount] = tempMeshObject.Colors[posIndex];

                    indices[indicesCount] = uniqueVertexCount ;
                    map.insert({ key, uniqueVertexCount});

                    indicesCount++;
                    uniqueVertexCount++;
                }
            }

            positionIndexOffset += Utils::SizeToUint32(tempMeshObject.Positions.size());
            normalsIndexOffset += Utils::SizeToUint32(tempMeshObject.Normals.size());
            texCoordsIndexOffset += Utils::SizeToUint32(tempMeshObject.TexCoords.size());

            out_Model.MeshObjects.push_back(Mesh(meshData));
        }
    }
}