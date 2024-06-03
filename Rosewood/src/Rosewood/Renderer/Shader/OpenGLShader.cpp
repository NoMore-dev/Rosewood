#pragma once

#include "rwpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Rosewood
{

    OpenGLShader::OpenGLShader(const char* vertexPath, const char* fragmentPath):
        m_RendererID(0)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file’s buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure const&)
        {
            RW_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");

            return;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            RW_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n {0}", infoLog);

            glDeleteShader(vertex);
            return;
        };
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            RW_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {0}", infoLog);

            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        };
        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertex);
        glAttachShader(m_RendererID, fragment);
        glLinkProgram(m_RendererID);
        // print linking errors if any
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
            RW_CORE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {0}", infoLog);

            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        }
        // delete shaders; they’re linked into our program and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind()
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind()
    {
        glUseProgram(0);
    }

    //void OpenGLShader::SetBool(const std::string& name, bool value) const
    //{
    //    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
    //}
    //void OpenGLShader::SetInt(const std::string& name, int value) const
    //{
    //    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    //}
    //void OpenGLShader::SetFloat(const std::string& name, float value) const
    //{
    //    glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
    //}
    //void OpenGLShader::SetVec3(const std::string& name, glm::vec3 value) const
    //{
    //    glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
    //}
    //void OpenGLShader::SetMat4(const std::string& name, glm::mat4 value) const
    //{
    //    unsigned int transformLoc = glGetUniformLocation(m_RendererID, name.c_str());
    //    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
    //}
}