#include "rwpch.h"
#include "Buffer.h"
#include "Rosewood/Renderer/Buffer/OpenGLBuffer.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood 
{

	// VertexBuffer //////////////////////////////////////////////////////////////

	Ref<VertexBuffer> VertexBuffer::Create(BYTE* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}


	// IndexBuffer //////////////////////////////////////////////////////////////

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}


	// StorageBuffer //////////////////////////////////////////////////////////////

	Ref<StorageBuffer> StorageBuffer::Create(BYTE* data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLStorageBuffer>(data, size);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	// UniformBuffer //////////////////////////////////////////////////////////////

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLUniformBuffer>(size, binding);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}