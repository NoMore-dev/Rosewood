#pragma once

#include "rwpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Rosewood
{

    OpenGLShader::OpenGLShader(const std::string& filepath, const std::string& name) :
        m_RendererID(0), m_Name(name)
    {
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

        CompileAndLink(vertexCode, fragmentCode);
    }

    OpenGLShader::OpenGLShader(const ShaderComponentPaths componentPaths, const std::string& name) :
        m_RendererID(0), m_Name(name)
    {
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
        
        CompileAndLink(vertexCode, fragmentCode);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }


    void OpenGLShader::CompileAndLink(const std::string vertexCode, const std::string fragmentCode)
    {
        unsigned int vertex, fragment;
        char infoLog[512];

        vertex = CompileShaderCode(vertexCode.c_str(), GL_VERTEX_SHADER, infoLog);
        if (!vertex)
        {
            RW_CORE_ERROR("{0}", infoLog);
            return;
        }

        fragment = CompileShaderCode(fragmentCode.c_str(), GL_FRAGMENT_SHADER, infoLog);
        if (!fragment)
        {
            RW_CORE_ERROR("{0}", infoLog);
            glDeleteShader(fragment);
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

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        m_SuccessfullyCompiled = true;
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

    void OpenGLShader::SplitSource(const std::string source, std::string& out_VertexCode, std::string& out_FragmentCode)
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

    int OpenGLShader::CompileShaderCode(const char* shaderCode, GLenum shaderType, char* infoLog)
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
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, const glm::int32 value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

}