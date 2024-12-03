#include "rwpch.h"
#include "Mesh.h"

#include <unordered_map>

namespace Rosewood
{
    MeshData::MeshData()
    {
    }

    MeshData::MeshData(const std::string& filepath, FileFormat fileFormat)
    {
        LoadFromFile(filepath, fileFormat);
    }

    void MeshData::LoadFromFile(const std::string& filepath,FileFormat fileFormat)
	{
        switch (fileFormat)
        {
        case Rosewood::MeshData::OBJ:
            MeshData::LoadFromOBJFile(filepath);
            break;
        case Rosewood::MeshData::CUSTOM:
            MeshData::LoadFromCustomFile(filepath);
            break;
        }

	}

    void MeshData::LoadFromOBJFile(const std::string& filepath)
    {
        std::ifstream fileStream;
        fileStream.open(filepath);

        RW_CORE_ASSERT(fileStream.is_open(), "Unable to open file : '" + filepath + "'");

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::ivec3> triangles;

        std::string tag;

        while (fileStream.good()) {

            fileStream >> tag;

            if (tag == "v") {
                std::string x, y, z;
                fileStream >> x; fileStream >> y; fileStream >> z;
                positions.push_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
            }

            else if (tag == "vn") {
                std::string x, y, z;
                fileStream >> x; fileStream >> y; fileStream >> z;
                normals.push_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
            }

            else if (tag == "vt") {
                std::string x, y;
                fileStream >> x; fileStream >> y;
                texCoords.push_back(glm::vec2(std::stof(x), std::stof(y)));
            }

            else if (tag == "f") {
                std::string a, b, c;
                fileStream >> a; fileStream >> b; fileStream >> c;

                std::string delimiter = "/";
                for (std::string str : {a, b, c}) {
                    uint32_t x, y, z;

                    size_t pos = 0;
                    pos = str.find(delimiter);
                    x = std::stoi(str.substr(0, pos));
                    str.erase(0, pos + delimiter.length());
                    pos = str.find(delimiter);
                    y = std::stoi(str.substr(0, pos));
                    str.erase(0, pos + delimiter.length());
                    z = std::stoi(str);

                    triangles.push_back(glm::ivec3(x, y, z));
                }
            }

            tag = "";
        }

        fileStream.close();
        
        uint32_t indexCount = (uint32_t)triangles.size();
        m_Indices = std::vector<uint32_t>(indexCount);

        std::unordered_map<std::string, uint32_t> map{};

        for (uint32_t i = 0; i < indexCount; i++) {

            std::string key = std::to_string(triangles[i].x) + "/" + std::to_string(triangles[i].y) + "/" + std::to_string(triangles[i].z);

            if (map.count(key))
            {
                m_Indices[i] = map.at(key); continue;
            }

            m_Indices[i] = (uint32_t)m_Vertices.size();
            map.insert({ key, (uint32_t)m_Vertices.size() });
            
            Vertex vert;

            vert.Position = positions[triangles[i].x - 1];

            uint16_t u = (uint16_t)std::round(65535.f * texCoords[triangles[i].y - 1].x);
            uint16_t v = (uint16_t)std::round(65535.f * texCoords[triangles[i].y - 1].y);
            vert.TexCoords[0] = u; vert.TexCoords[1] = v;
            //vert.TexCoords = texCoords[triangles[i].y - 1];

            //glm::vec3 normal = glm::normalize(normals[triangles[i].z - 1]);
            //int32_t nx = normal.x > 0 ? (int32_t)std::round(511.f * normal.x) : (int32_t)std::round(512.f * normal.x);
            //int32_t ny = normal.y > 0 ? (int32_t)std::round(511.f * normal.y) : (int32_t)std::round(512.f * normal.y);
            //int32_t nz = normal.z > 0 ? (int32_t)std::round(511.f * normal.z) : (int32_t)std::round(512.f * normal.z);
            //vert.Normal = (nz << 20) | (ny << 10) | (nx << 0);
            vert.Normal = normals[triangles[i].z - 1];

            uint32_t r = (uint32_t)std::round(1023.f * .0f);
            uint32_t g = (uint32_t)std::round(1023.f * .0f);
            uint32_t b = (uint32_t)std::round(1023.f * .0f);
            uint32_t a = (uint32_t)(3.f * 1.f);
            vert.Color = (a << 30) | (b << 20) | (g << 10) | (r << 0);

            m_Vertices.push_back(vert);
        }
    }

    void MeshData::LoadFromCustomFile(const std::string& filepath)
    {
        char* buffer;

        std::basic_ifstream<char> fileStream(filepath, std::ios::binary);
        RW_CORE_ASSERT(fileStream.is_open(), "Unable to open file : '" + filepath + "'");

        fileStream.seekg(0, std::ios::end);
        size_t length = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        buffer = new char[length];
        fileStream.read(buffer, length);

        fileStream.close();

        uint32_t vertexCount = *(uint32_t*)buffer;
        uint32_t indexCount = *(uint32_t*)(buffer+sizeof(uint32_t));

        Vertex* vertices_buffer = (Vertex*)(buffer + 2 * sizeof(uint32_t));
        m_Vertices = std::vector<Vertex>(vertices_buffer, vertices_buffer + vertexCount);

        uint32_t* indices_buffer = (uint32_t*)(buffer + 2 * sizeof(uint32_t) + sizeof(Vertex) * vertexCount);
        m_Indices = std::vector<uint32_t>(indices_buffer, indices_buffer + indexCount);

        delete[] buffer;
    }

    Ref<VertexArray> MeshData::MakeVertexArray()
    {
        Rosewood::BufferLayout layout = {
        	{ Rosewood::ShaderDataType::Float3,				"a_Position"		},
        	{ Rosewood::ShaderDataType::Float3,				"a_Normal"			},
        	{ Rosewood::ShaderDataType::UShort2,			"a_UV"				},
        	{ Rosewood::ShaderDataType::Packed_2ZYX_UInt,	"a_Color",		true},
        };

        Ref<VertexArray> VAO = Rosewood::VertexArray::Create();
        VAO->Bind();

		Rosewood::Ref<Rosewood::VertexBuffer> vb = Rosewood::VertexBuffer::Create((BYTE*)m_Vertices.data(), GetVertexCount() * sizeof(Rosewood::MeshData::Vertex));
		vb->SetLayout(layout);
		Rosewood::Ref<Rosewood::IndexBuffer> ib = Rosewood::IndexBuffer::Create(m_Indices.data(), GetIndexCount());

        VAO->AddVertexBuffer(vb);
        VAO->SetIndexBuffer(ib);

        return VAO;
    }

    void MeshData::SaveDataToCustomFormat(const std::string& filepath) const
    {
        std::ofstream file;
        file.open(filepath, std::ios_base::out | std::ios_base::binary);

        uint32_t vertexCount = (uint32_t)m_Vertices.size();
        uint32_t indexCount = (uint32_t)m_Indices.size();

        file.write((const char*)&vertexCount, sizeof(uint32_t));
        file.write((const char*)&indexCount, sizeof(uint32_t));

        const float* v = &m_Vertices[0].Position.x;
        file.write((const char*)v, vertexCount * sizeof(Vertex));
        const uint32_t* i = &m_Indices[0];
        file.write((const char*)i, indexCount * sizeof(uint32_t));

        file.close();
    }

}