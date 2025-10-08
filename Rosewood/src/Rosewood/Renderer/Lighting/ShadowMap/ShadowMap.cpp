#include "rwpch.h"
#include "ShadowMap.h"
#include "OpenGLShadowMap.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood
{
	Ref<ShadowMap> ShadowMap::Create(ShadowMapSpecification spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShadowMap>(spec);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}