#pragma once

#include "rwpch.h"
#include "OpenGLShader.h"
#include "Rosewood/Renderer/Shader/SPIRVShaderDataType.h"


#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Rosewood
{

    OpenGLShader::OpenGLShader(const std::string& filepath, const std::string& name) :
        m_RendererID(0)
    {
        m_Name = name;

        std::string source;
        try
        {
            source = ReadFile(filepath);
        }
        catch (std::ifstream::failure const&)
        {
            RW_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
            return;
        }

        std::string vertexCode;
        std::string fragmentCode;
        SplitSource(source, vertexCode, fragmentCode);
        RW_CORE_ASSERT(!vertexCode.empty(), "Missing vertex shader!");
        RW_CORE_ASSERT(!fragmentCode.empty(), "Missing fragment shader!");


        std::vector<uint32_t> vertexVulkanSpirv;
        std::vector<uint32_t> fragmentVulkanSpirv;
        MakeVulkanSpirvFromVulkanSources(vertexCode, fragmentCode, vertexVulkanSpirv, fragmentVulkanSpirv);
        std::string vertexOpenGLSource;
        std::string fragmentOpenGLSource;
        MakeOpenGLSourcesFromVulkanSpirv(vertexVulkanSpirv, fragmentVulkanSpirv, vertexOpenGLSource, fragmentOpenGLSource);

        CompileAndLinkFromOpenGLSources(vertexOpenGLSource, fragmentOpenGLSource);

        ReflectShaderResources(fragmentVulkanSpirv);

        InitializeMaterialDataUniformBuffer();
    }

    OpenGLShader::OpenGLShader(const ShaderComponentPaths& componentPaths, const std::string& name) :
        m_RendererID(0)
    {
        m_Name = name;

        std::string vertexCode;
        try
        {
            vertexCode = ReadFile(componentPaths.VertexPath);
        }
        catch (std::ifstream::failure const&)
        {
            RW_CORE_ERROR("ERROR::SHADER::VERTEX::FILE_NOT_SUCCESFULLY_READ");
            return;
        }

        std::string fragmentCode;
        try
        {
            fragmentCode = ReadFile(componentPaths.FragmentPath);
        }
        catch (std::ifstream::failure const&)
        {
            RW_CORE_ERROR("ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESFULLY_READ");
            return;
        }
        

        std::vector<uint32_t> vertexVulkanSpirv;
        std::vector<uint32_t> fragmentVulkanSpirv;
        MakeVulkanSpirvFromVulkanSources(vertexCode, fragmentCode, vertexVulkanSpirv, fragmentVulkanSpirv);
        std::string vertexOpenGLSource;
        std::string fragmentOpenGLSource;
        MakeOpenGLSourcesFromVulkanSpirv(vertexVulkanSpirv, fragmentVulkanSpirv, vertexOpenGLSource, fragmentOpenGLSource);

        CompileAndLinkFromOpenGLSources(vertexOpenGLSource, fragmentOpenGLSource);

        ReflectShaderResources(fragmentVulkanSpirv);

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
            uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            int memberCount = bufferType.member_types.size();

            if (resource.name == "Material")
            {
                for (int i = 0; i < memberCount; i++)
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

    void OpenGLShader::SplitSource(const std::string& source, std::string& out_VertexCode, std::string& out_FragmentCode)
    {
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            RW_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            type = type.substr(0, type.find(" "));

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);

            if (type == "vertex")
                if (!out_VertexCode.empty())
                    RW_CORE_WARN("Shader type {0} already loaded, new occurence ignored!", type);
                else
                    out_VertexCode = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

            else if (type == "fragment" || type == "pixel")
                if (!out_FragmentCode.empty())
                    RW_CORE_WARN("Shader type {0} already loaded, new occurence ignored!", type);
                else
                    out_FragmentCode = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

            else
                RW_CORE_WARN("Unknown shader type : {0}", type);
        }
    }

    void OpenGLShader::MakeVulkanSpirvFromVulkanSources(const std::string& vertexVulkanSource, const std::string& fragmentVulkanSource, std::vector<uint32_t>& out_vertexVulkanSpirv, std::vector<uint32_t>& out_fragmentVulkanSpirv)
    {
        std::vector<uint32_t> vertexVulkanSpirv;
        std::vector<uint32_t> fragmentVulkanSpirv;

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(vertexVulkanSource, shaderc_glsl_vertex_shader, "VERTEX_SHADER_VULCAN_SPIRV", options);
        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            RW_CORE_ERROR(module.GetErrorMessage());
            return;
        }
        out_vertexVulkanSpirv = { module.begin(), module.end() };

        module = compiler.CompileGlslToSpv(fragmentVulkanSource, shaderc_glsl_fragment_shader, "FRAGMENT_SHADER_VULCAN_SPIRV", options);
        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            RW_CORE_ERROR(module.GetErrorMessage());
            return;
        }
        out_fragmentVulkanSpirv = { module.begin(), module.end() };
    }


    void OpenGLShader::MakeOpenGLSourcesFromVulkanSpirv(const std::vector<uint32_t>& vertexVulkanSpirv, const std::vector<uint32_t>& fragmentVulkanSpirv, std::string& out_vertexOpenGLSource, std::string& out_fragmentOpenGLSource)
    {
        spirv_cross::CompilerGLSL glslCompilerVertex(vertexVulkanSpirv);
        out_vertexOpenGLSource = glslCompilerVertex.compile();

        spirv_cross::CompilerGLSL glslCompilerFragment(fragmentVulkanSpirv);
        out_fragmentOpenGLSource = glslCompilerFragment.compile();

        {
            //// compile from opengl source to opengl spirv
            //    shaderc::Compiler compiler2;
            //    shaderc::CompileOptions options2;
            //    options2.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

            //    std::vector<uint32_t> vertexOpenGLSpirv;
            //    std::vector<uint32_t> fragmentOpenGLSpirv;

            //    module = compiler2.CompileGlslToSpv(vertexOpenGLSource, shaderc_glsl_vertex_shader, "VERTEX_SHADER_OPENGL_SPIRV", options2);
            //    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
            //    {
            //        RW_CORE_ERROR(module.GetErrorMessage());
            //        return;
            //    }
            //    vertexOpenGLSpirv = std::vector<uint32_t>(module.cbegin(), module.cend());

            //    module = compiler2.CompileGlslToSpv(fragmentOpenGLSource, shaderc_glsl_fragment_shader, "FRAGMENT_SHADER_OPENGL_SPIRV", options2);
            //    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
            //    {
            //        RW_CORE_ERROR(module.GetErrorMessage());
            //        return;
            //    }
            //    fragmentOpenGLSpirv = std::vector<uint32_t>(module.cbegin(), module.cend());


            //// make opengl shader program from opengl spirv
            //    GLuint m_RendererID = glCreateProgram();

            //    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            //    glShaderBinary(1, &vertexShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexOpenGLSpirv.data(), vertexOpenGLSpirv.size() * sizeof(uint32_t));
            //    glSpecializeShader(vertexShaderID, "main", 0, nullptr, nullptr);

            //    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
            //    glShaderBinary(1, &fragmentShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentOpenGLSpirv.data(), fragmentOpenGLSpirv.size() * sizeof(uint32_t));
            //    glSpecializeShader(fragmentShaderID, "main", 0, nullptr, nullptr);

            //    glAttachShader(m_RendererID, vertexShaderID);
            //    glAttachShader(m_RendererID, fragmentShaderID);

            //    glLinkProgram(m_RendererID);

            //    int success;
            //    char* infoLog = new char[512];
            //    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
            //    if (!success)
            //    {
            //        glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
            //        RW_CORE_ERROR("{0}", infoLog);

            //        glDeleteProgram(m_RendererID);

            //        glDeleteShader(vertexShaderID);
            //        glDeleteShader(fragmentShaderID);

            //        return;
            //    }

            //    glDetachShader(m_RendererID, vertexShaderID);
            //    glDetachShader(m_RendererID, fragmentShaderID);

            //    glDeleteShader(vertexShaderID);
            //    glDeleteShader(fragmentShaderID);

            //    m_SuccessfullyCompiled = true;
        }
    }


    void OpenGLShader::CompileAndLinkFromOpenGLSources(const std::string& vertexCode, const std::string& fragmentCode)
    {
        unsigned int vertex, fragment;
        char infoLog[512];

        vertex = CompileOpenGLSource(vertexCode.c_str(), GL_VERTEX_SHADER, infoLog);
        if (!vertex)
        {
            RW_CORE_ERROR("{0}", infoLog);
            return;
        }

        fragment = CompileOpenGLSource(fragmentCode.c_str(), GL_FRAGMENT_SHADER, infoLog);
        if (!fragment)
        {
            RW_CORE_ERROR("{0}", infoLog);
            glDeleteShader(vertex);
            return;
        }

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertex);
        glAttachShader(m_RendererID, fragment);
        glLinkProgram(m_RendererID);

        int success;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
            RW_CORE_ERROR("{0}", infoLog);

            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        }

        glDetachShader(m_RendererID, vertex);
        glDetachShader(m_RendererID, fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);

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