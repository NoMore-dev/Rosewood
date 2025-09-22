#include "rwpch.h"
#include "ComputeShader.h"
#include "Rosewood/Renderer/Renderer.h"
#include "OpenGLComputeShader.h"

namespace Rosewood
{
	Ref<ComputeShader> ComputeShader::Create(const ComputeShaderSpecification& shaderSpec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLComputeShader>(shaderSpec);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}
