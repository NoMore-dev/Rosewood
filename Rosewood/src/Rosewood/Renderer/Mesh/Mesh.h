#pragma once

#include "Rosewood/Renderer/Buffer/Buffer.h"
#include "Rosewood/Renderer/VertexArray/VertexArray.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>
#include <array>
#include <optional>

namespace Rosewood
{
	struct MeshData
	{
		struct SurfaceData
		{
			SurfaceData() = default;
			~SurfaceData() = default;
			SurfaceData(const SurfaceData&) = default;
			SurfaceData(SurfaceData&&) = default;

			std::vector<uint32_t> Indices;

			std::vector<glm::vec3> Positions;
			std::vector<glm::vec3> Normals;
			std::vector<glm::vec2> TexCoords;
			std::vector<glm::vec3> Colors;
		};

		MeshData() = default;
		~MeshData() = default;
		MeshData(const MeshData&) = default;
		MeshData(MeshData&&) = default;

		std::string ObjectName = "";

		std::vector<SurfaceData> Surfaces;
		std::vector<std::string> SlotNames;
	};

	class Mesh
	{
	public:
		Mesh(const MeshData& data);
		Mesh(MeshData&& data);
		~Mesh() = default;

		std::vector<Ref<VertexArray>> MakeVertexArrays();
		std::vector<MeshData::SurfaceData>& GetSurfaces();


	public:
		MeshData m_MeshData;
	};


	class ModelImport 
	{
	public:
		enum FileFormat { OBJ };

		struct ImportedModel
		{
			ImportedModel() = default;
			~ImportedModel() = default;
			ImportedModel(const ImportedModel&) = delete;
			ImportedModel(ImportedModel&&) = default;

			std::vector<Mesh> MeshObjects;
		};

	public:
		ModelImport() = delete;

		static ImportedModel Import(const std::string& filepath, FileFormat fileFormat);

	private:
		static void ImportFromOBJ(const std::string& filepath, ImportedModel& out_Model);

	};
}