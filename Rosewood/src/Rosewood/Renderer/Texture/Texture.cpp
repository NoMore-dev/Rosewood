#include "rwpch.h"
#include "Texture.h"

#include "Rosewood/Renderer/Renderer.h"
#include "OpenGLTexture.h"

namespace Rosewood {

	Ref<Texture2D> Texture2D::Create(const std::string& path, const TextureSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path, spec);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}

