#include "rwpch.h"
#include "VertexArray.h"
#include "Rosewood/Renderer/VertexArray/OpenGLVertexArray.h"
#include "Rosewood/Renderer/Renderer.h"


namespace Rosewood 
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}