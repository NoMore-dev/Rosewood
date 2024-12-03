#include "rwpch.h"
#include "Batch.h"

namespace Rosewood
{
	Batch::Batch(const std::vector<MeshData>& meshes, const std::vector<glm::mat4>& transforms, BufferLayout layout)
	{
		m_VertexArray = VertexArray::Create();
		m_VertexCounts = std::vector<uint32_t>(meshes.size());
		m_IndexCounts = std::vector<uint32_t>(meshes.size());
		m_ObjectCount = (uint32_t)meshes.size();
		
		uint32_t totalVertexCount = 0;
		uint32_t totalIndexCount = 0;

		for (int i = 0; i < meshes.size(); i++)
		{
			totalVertexCount += meshes[i].GetVertexCount();
			totalIndexCount += meshes[i].GetIndexCount();

			m_VertexCounts[i] = meshes[i].GetVertexCount();
			m_IndexCounts[i] = meshes[i].GetIndexCount();
		}

		BYTE* vertices = new BYTE[totalVertexCount * layout.GetStride()];
		uint32_t* indices = new uint32_t[totalIndexCount];

		BYTE* vert_dest_ptr = vertices;
		BYTE* ind_dest_ptr = (BYTE*)indices;
		for (const MeshData& mesh : meshes)
		{
			BYTE* vert_src_ptr = (BYTE*)mesh.m_Vertices.data();
			uint32_t vert_byte_count = mesh.GetVertexCount() * layout.GetStride();
			memcpy(vert_dest_ptr, vert_src_ptr, vert_byte_count);
			vert_dest_ptr += vert_byte_count;

			BYTE* ind_src_ptr = (BYTE*)mesh.m_Indices.data();
			uint32_t ind_byte_count = mesh.GetIndexCount() * sizeof(uint32_t);
			memcpy(ind_dest_ptr, ind_src_ptr, ind_byte_count);
			ind_dest_ptr += ind_byte_count;
		}    
		 

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, totalVertexCount * layout.GetStride());
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, totalIndexCount);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_TransformBuffer = StorageBuffer::Create((BYTE*)&transforms[0], transforms.size() * sizeof(glm::mat4));




		m_Commands = new DrawElementsIndirectCommand[m_ObjectCount];
		uint32_t firstIndex = 0;
		uint32_t baseVertex = 0;
		for (uint32_t i = 0; i < m_ObjectCount; i++)
		{
			m_Commands[i] = DrawElementsIndirectCommand{ GetIndexCount(i), 1, firstIndex, baseVertex, 0 };
			firstIndex += GetIndexCount(i);
			baseVertex += GetVertexCount(i);
		}
	}
}