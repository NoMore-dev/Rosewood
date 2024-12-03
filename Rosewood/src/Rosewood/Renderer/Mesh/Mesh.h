#pragma once

#include "Rosewood/Renderer/VertexArray/VertexArray.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>


namespace Rosewood
{
	class MeshData
	{
	public:
		//struct Vertex
		//{
		//	glm::vec3 Position = glm::vec3{};
		//};

		struct Vertex
		{
			glm::vec3 Position = glm::vec3{};
			glm::vec3 Normal = glm::vec3{};
			uint16_t TexCoords[2] = { 0, 0 };
			uint32_t Color = 0;
		};

		enum FileFormat { OBJ, CUSTOM };

	public:
		MeshData();
		MeshData(const std::string& filepath, FileFormat fileFormat);
		~MeshData() = default;

		Ref<VertexArray> MakeVertexArray();

		void SaveDataToCustomFormat(const std::string& filepath) const;

		uint32_t GetVertexCount() const { return (uint32_t)m_Vertices.size(); };
		uint32_t GetIndexCount() const { return (uint32_t)m_Indices.size(); };

	private:
		void LoadFromFile(const std::string& filepath, FileFormat fileFormat);
		void LoadFromOBJFile(const std::string& filepath);
		void LoadFromCustomFile(const std::string& filepath);

	public:
		std::vector<Vertex> m_Vertices = std::vector<Vertex>();
		std::vector<uint32_t> m_Indices = std::vector<uint32_t>();
	};

}