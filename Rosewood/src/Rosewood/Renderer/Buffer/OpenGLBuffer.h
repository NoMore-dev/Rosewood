#pragma once

#include "Rosewood/Renderer/Buffer/Buffer.h"

namespace Rosewood
{

	// VertexBuffer //////////////////////////////////////////////////////////////

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_RendererID;

	};


	// IndexBuffer //////////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		~OpenGLIndexBuffer();

		void Bind() override;
		void Unbind() override;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}