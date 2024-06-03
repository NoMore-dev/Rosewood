#pragma once

#include "rwpch.h"
#include "Shader.h"

#include "Rosewood/Renderer/Shader/OpenGLShader.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood
{
	Shader* Shader::Create(const char* vertexPath, const char* fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLShader(vertexPath, fragmentPath);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}