#pragma once

#include"Rosewood/Renderer/Mesh/Mesh.h"
#include"Rosewood/Renderer/VertexArray/VertexArray.h"
#include"Rosewood/Renderer/Buffer/Buffer.h"

namespace Rosewood
{

	class Batch
	{
		struct DrawElementsIndirectCommand
		{
			uint32_t  count;
			uint32_t  instanceCount;
			uint32_t  firstIndex;
			uint32_t  baseVertex;
			uint32_t  baseInstance;
		};

	public:
		Batch(const std::vector<MeshData>& meshes, const std::vector<glm::mat4>& transforms, BufferLayout layout);
		~Batch() = default;

		Ref<VertexArray> GetVertexArray() { return m_VertexArray; }
		Ref<StorageBuffer> GetTransformBuffer(){ return m_TransformBuffer; }
		uint32_t GetObjectCount() const { return m_ObjectCount; }
		uint32_t GetVertexCount(uint32_t object) const { return m_VertexCounts[object]; }
		uint32_t GetIndexCount(uint32_t object) const { return m_IndexCounts[object]; }

		DrawElementsIndirectCommand* GetCommands() { return m_Commands; }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<StorageBuffer> m_TransformBuffer;
		DrawElementsIndirectCommand* m_Commands;

		uint32_t m_ObjectCount = 0;
		std::vector<uint32_t> m_VertexCounts;
		std::vector<uint32_t> m_IndexCounts;
	};

}