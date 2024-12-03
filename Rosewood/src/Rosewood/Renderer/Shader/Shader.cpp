#pragma once

#include "rwpch.h"
#include "Shader.h"

#include "Rosewood/Renderer/Shader/OpenGLShader.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood
{
	Ref<Shader> Shader::Create(const std::string& filepath, const std::string& name)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath, name);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const ShaderComponentPaths componentPaths, const std::string& name)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(componentPaths, name);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	void Shader::Unbind() const
	{
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath, const std::string& name)
	{
		Ref<Shader> shader = Shader::Create(filepath, name);

		if (shader->IsCompiled() && Add(shader))
			return shader;

		RW_CORE_ERROR("ERROR::SHADERLIBRARY\n	Failed to load shader : '{0}'!", name);
		return nullptr;
	}

	Ref<Shader> ShaderLibrary::Load(const ShaderComponentPaths componentPaths, const std::string& name)
	{
		Ref<Shader> shader = Shader::Create(componentPaths, name);

		if (shader->IsCompiled() && Add(shader))
			return shader;

		RW_CORE_ERROR("ERROR::SHADERLIBRARY\n	Failed to load shader : '{0}'!", name);
		return nullptr;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) const
	{
		try
		{
			return m_Shaders.at(name);
		}
		catch(std::out_of_range)
		{
			RW_CORE_ERROR("ERROR::SHADERLIBRARY\n	The shader library doesn't contains the requested shader's name : '{0}' !", name);
			return nullptr;
		}
	}

	int ShaderLibrary::Add(const Ref<Shader> shader)
	{	
		if (m_Shaders.count(shader->GetName())) 
		{
			RW_CORE_ERROR("ERROR::SHADERLIBRARY\n	The shader library already contains a shader for this name : '{0}' !", shader->GetName());
			return 0;
		}

		m_Shaders.insert({ shader->GetName(), shader });
		return 1;
	}
}