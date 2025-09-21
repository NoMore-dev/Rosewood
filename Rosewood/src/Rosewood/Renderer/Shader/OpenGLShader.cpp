#pragma once

#include "rwpch.h"
#include "OpenGLShader.h"
#include "Rosewood/Renderer/Shader/SPIRVShaderDataType.h"
#include "Rosewood/Utils/Conversions.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Rosewood
{
    static shaderc_shader_kind ShaderStageKind(ShaderStage stage)
    {
        switch (stage)
        {
        case Rosewood::ShaderStage::Vertex:
            return shaderc_shader_kind::shaderc_glsl_vertex_shader;
        case Rosewood::ShaderStage::Fragment:
            return shaderc_shader_kind::shaderc_glsl_fragment_shader;
        case Rosewood::ShaderStage::Geometry:
            return shaderc_shader_kind::shaderc_glsl_geometry_shader;
        case Rosewood::ShaderStage::Hull:
            return shaderc_shader_kind::shaderc_glsl_tess_control_shader;
        case Rosewood::ShaderStage::Domain:
            return shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader;
        default:
            break;
        }

        RW_CORE_ASSERT(false, " Invalid shader stage! ")
        return shaderc_shader_kind(0);
    }

    static GLenum ShaderStageOpenGLenum(ShaderStage stage)
    {
        switch (stage)
        {
        case Rosewood::ShaderStage::Vertex:
            return GL_VERTEX_SHADER;
        case Rosewood::ShaderStage::Fragment:
            return GL_FRAGMENT_SHADER;
        case Rosewood::ShaderStage::Geometry:
            return GL_GEOMETRY_SHADER;
        case Rosewood::ShaderStage::Hull:
            return GL_TESS_CONTROL_SHADER;
        case Rosewood::ShaderStage::Domain:
            return GL_TESS_EVALUATION_SHADER;
        default:
            break;
        }

        RW_CORE_ASSERT(false, " Invalid shader stage! ")
        return GLenum(0);
    }


    OpenGLShader::OpenGLShader(const ShaderSpecification& shaderSpec) :
        m_RendererID(0)
    {
        m_Name = shaderSpec.Name;

        std::vector<std::pair<ShaderStage, std::string>> shaderStagesPaths;
        shaderStagesPaths.push_back({ ShaderStage::Vertex, shaderSpec.VertexPath });
        shaderStagesPaths.push_back({ ShaderStage::Fragment, shaderSpec.FragmentPath });
        if (shaderSpec.GeometryPath.has_value()) shaderStagesPaths.push_back({ ShaderStage::Geometry, shaderSpec.GeometryPath.value()});
        if (shaderSpec.HullPath.has_value()) shaderStagesPaths.push_back({ ShaderStage::Hull, shaderSpec.HullPath.value() });
        if (shaderSpec.DomainPath.has_value()) shaderStagesPaths.push_back({ ShaderStage::Domain, shaderSpec.DomainPath.value() });

        std::vector<std::pair<ShaderStage, std::string>> shaderStagesVulkanSources;
        for (auto& path : shaderStagesPaths)
        {
            try
            {
                shaderStagesVulkanSources.push_back( { path.first, ReadFile(path.second) } );
            }
            catch (std::ifstream::failure const&)
            {
                RW_CORE_ERROR("ERROR::SHADER::" + ShaderStageString(path.first) + "::FILE_NOT_SUCCESFULLY_READ");
                return;
            }
        }
        
        std::vector<std::pair<ShaderStage, std::vector<uint32_t>>> shaderStagesVulkanSpirv;
        for (auto& source : shaderStagesVulkanSources)
        {
            std::vector<uint32_t> spirv;
            MakeVulkanSpirvFromVulkanSource(source.first, source.second, spirv);
            shaderStagesVulkanSpirv.push_back({ source.first, spirv });
        }
       

        std::vector<std::pair<ShaderStage, std::string>> shaderStagesOpenGLSources;
        for (auto& spirv : shaderStagesVulkanSpirv)
        {
            std::string source;
            MakeOpenGLSourcesFromVulkanSpirv(spirv.first, spirv.second, source);
            shaderStagesOpenGLSources.push_back({ spirv.first, source });
        }


        CompileAndLinkFromOpenGLSources(shaderStagesOpenGLSources);

        ReflectShaderResources(shaderStagesVulkanSpirv[1].second);


        InitializeMaterialDataUniformBuffer();
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }



    void OpenGLShader::ReflectShaderResources(const std::vector<uint32_t>& fragmentVulkanSpirv)
    {
        std::vector<BufferElement> materialBufferLayoutInitVector;

        spirv_cross::Compiler compiler(fragmentVulkanSpirv);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        for (const auto& resource : resources.uniform_buffers)
        {
            const auto& bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize = Utils::SizeToUint32(compiler.get_declared_struct_size(bufferType));
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t memberCount = Utils::SizeToUint32(bufferType.member_types.size());

            if (resource.name == "Material")
            {
                for (uint32_t i = 0; i < memberCount; i++)
                {
                    materialBufferLayoutInitVector.push_back({ SPIRTypeToShaderDataType(compiler.get_type(compiler.get_type(resource.base_type_id).member_types[i])), compiler.get_member_name(resource.base_type_id, i) });
                }
                break;
            }
        }

        m_MaterialDataLayout = BufferLayout( LayoutStandard::STD140, materialBufferLayoutInitVector );

        for (const auto& resource : resources.sampled_images)
        {
            m_TextureBindingSlots.push_back({ compiler.get_decoration(resource.id, spv::DecorationBinding), resource.name });
        }
    }

    void OpenGLShader::InitializeMaterialDataUniformBuffer()
    {
        m_MaterialDataUniformBuffer = UniformBuffer::Create(m_MaterialDataLayout.GetStride());
    }


    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string source;
        std::ifstream file;
        // ensure ifstream objects can throw exceptions:
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            file.open(filepath);
            std::stringstream stream;
            // read file’s buffer contents into streams
            stream << file.rdbuf();
            // close file handlers
            file.close();
            // convert stream into string
            source = stream.str();
        }
        catch (std::ifstream::failure const&)
        {
            throw;
        }

        return source;
    }

    void OpenGLShader::MakeVulkanSpirvFromVulkanSource(ShaderStage stage, const std::string& vulkanSource, std::vector<uint32_t>& out_VulkanSpirv)
    {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(vulkanSource, ShaderStageKind(stage), (ShaderStageString(stage) + "_SHADER_VULCAN_SPIRV").c_str(), options);
        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            RW_CORE_ERROR(module.GetErrorMessage());
            return;
        }
        out_VulkanSpirv = { module.begin(), module.end() };

    }


    void OpenGLShader::MakeOpenGLSourcesFromVulkanSpirv(ShaderStage stage, const std::vector<uint32_t>& vulkanSpirv, std::string& out_OpenGLSource)
    {
        spirv_cross::CompilerGLSL glslCompiler(vulkanSpirv);
        out_OpenGLSource = glslCompiler.compile();
    }


    void OpenGLShader::CompileAndLinkFromOpenGLSources(const std::vector<std::pair<ShaderStage, std::string>>& sources)
    {
        std::vector<unsigned int> stageIDs;
        char infoLog[512];

        for (auto& source : sources)
        {
            stageIDs.push_back(CompileOpenGLSource(source.second.c_str(), ShaderStageOpenGLenum(source.first), infoLog));
            if (!stageIDs.back())
            {
                RW_CORE_ERROR("{0}", infoLog);
                return;
            }
        }

        m_RendererID = glCreateProgram();
        for (auto& id : stageIDs)
            glAttachShader(m_RendererID, id);
        glLinkProgram(m_RendererID);

        int success;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
            RW_CORE_ERROR("{0}", infoLog);

            for (auto& id : stageIDs)
                glDeleteShader(id);

            return;
        }
        for (auto& id : stageIDs)
        {
            glDetachShader(m_RendererID, id);
            glDeleteShader(id);
        }

        m_SuccessfullyCompiled = true;
    }

    int OpenGLShader::CompileOpenGLSource(const char* shaderCode, GLenum shaderType, char* infoLog)
    {
        unsigned int shaderID;

        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shaderCode, NULL);
        glCompileShader(shaderID);

        int success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
            glDeleteShader(shaderID);
            return 0;
        };

        return shaderID;
    }



    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
        m_MaterialDataUniformBuffer->BindToBindingPoint(2);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }
}