#include "rwpch.h"
#include "Shader.h"

#include "Rosewood/Renderer/Shader/OpenGLShader.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood
{
	Ref<Shader> Shader::Create(const ShaderSpecification& shaderSpec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: RW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(shaderSpec);
		}

		RW_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}



	std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::s_Shaders = std::unordered_map<std::string, Ref<Shader>>();


	Ref<Shader> ShaderLibrary::Load(const ShaderSpecification& shaderSpec)
	{
		Ref<Shader> shader = Shader::Create(shaderSpec);

		if (shader->IsCompiled() && Add(shader))
			return shader;

		RW_CORE_ERROR("ERROR::SHADERLIBRARY\n	Failed to load shader : '{0}'!", shaderSpec.Name);
		return nullptr;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		try
		{
			return s_Shaders.at(name);
		}
		catch(std::out_of_range)
		{
			RW_CORE_ERROR("ERROR::SHADERLIBRARY\n	The shader library doesn't contains the requested shader's name : '{0}' !", name);
			return nullptr;
		}
	}

	int ShaderLibrary::Add(const Ref<Shader> shader)
	{	
		if (s_Shaders.count(shader->GetName())) 
		{
			RW_CORE_ERROR("ERROR::SHADERLIBRARY\n	The shader library already contains a shader for this name : '{0}' !", shader->GetName());
			return 0;
		}

		s_Shaders.insert({ shader->GetName(), shader });
		return 1;
	}
}