#include "rwpch.h"
#include "Buffer.h"
#include "Rosewood/Renderer/Buffer/OpenGLBuffer.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood 
{

	// VertexBuffer //////////////////////////////////////////////////////////////

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}


	// IndexBuffer //////////////////////////////////////////////////////////////

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}