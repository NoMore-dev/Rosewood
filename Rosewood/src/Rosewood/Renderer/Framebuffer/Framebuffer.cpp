#include "rwpch.h"
#include "Framebuffer.h"
#include "OpenGLFramebuffer.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood
{
	Ref<Framebuffer> Framebuffer::Create(FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}