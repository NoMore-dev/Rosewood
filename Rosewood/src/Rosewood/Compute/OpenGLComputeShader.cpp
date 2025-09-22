#include "rwpch.h"
#include "OpenGLComputeShader.h"
#include "Rosewood/Utils/File.h"

namespace Rosewood
{
    OpenGLComputeShader::OpenGLComputeShader(const ComputeShaderSpecification& shaderSpec)
    {
        m_Spec = shaderSpec;

        std::string source;
        try
        {
            source = Utils::ReadFile(shaderSpec.Path);
        }
        catch (std::ifstream::failure const&)
        {
            RW_CORE_ERROR("ERROR::SHADER::COMPUTE::FILE_NOT_SUCCESFULLY_READ");
            return;
        }

        CompileAndLinkFromOpenGLSources(source);
    }


    void OpenGLComputeShader::CompileAndLinkFromOpenGLSources(const std::string& source)
    {
        unsigned int shaderID;
        char infoLog[512];

        shaderID = CompileOpenGLSource(source.c_str(), infoLog);
        if (!shaderID)
        {
            RW_CORE_ERROR("{0}", infoLog);
            return;
        }

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, shaderID);
        glLinkProgram(m_RendererID);

        int success;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
            RW_CORE_ERROR("{0}", infoLog);

            glDeleteShader(shaderID);

            return;
        }
        glDetachShader(m_RendererID, shaderID);
        glDeleteShader(shaderID);

        m_SuccessfullyCompiled = true;
}

    int OpenGLComputeShader::CompileOpenGLSource(const char* shaderSource, char* infoLog)
    {
        unsigned int shaderID;

        shaderID = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(shaderID, 1, &shaderSource, NULL);
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


    void OpenGLComputeShader::DispatchCompute() const
    {
        Bind();
        glDispatchCompute(m_Spec.WorkGroupCount.x, m_Spec.WorkGroupCount.y, m_Spec.WorkGroupCount.z);
    }

    void OpenGLComputeShader::SetBarrier() const
    {
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    void OpenGLComputeShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLComputeShader::Unbind() const
    {
        glUseProgram(0);
    }

}