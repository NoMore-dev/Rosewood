#pragma once

#include "Rosewood/Renderer/VertexArray/VertexArray.h"

namespace Rosewood {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const Rosewood::Ref<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const Rosewood::Ref<IndexBuffer>& indexBuffer) override;

		const std::vector<Rosewood::Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		const Rosewood::Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<Rosewood::Ref<VertexBuffer>> m_VertexBuffers;
		Rosewood::Ref<IndexBuffer> m_IndexBuffer;
	};

}