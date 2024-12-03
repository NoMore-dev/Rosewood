#pragma once

#include "Rosewood/Renderer/Shader/Shader.h"

#include <glad/glad.h>

namespace Rosewood
{
    class OpenGLShader : public Shader
    {
    public:
        static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Float:             return GL_FLOAT;
            case ShaderDataType::Float2:            return GL_FLOAT;
            case ShaderDataType::Float3:            return GL_FLOAT;
            case ShaderDataType::Float4:            return GL_FLOAT;
            case ShaderDataType::Mat3:              return GL_FLOAT;
            case ShaderDataType::Mat4:              return GL_FLOAT;
            case ShaderDataType::Int:               return GL_INT;
            case ShaderDataType::Int2:              return GL_INT;
            case ShaderDataType::Int3:              return GL_INT;
            case ShaderDataType::Int4:              return GL_INT;
            case ShaderDataType::UShort2:           return GL_UNSIGNED_SHORT;
            case ShaderDataType::Bool:              return GL_BOOL;
            case ShaderDataType::Packed_2ZYX_UInt:  return GL_UNSIGNED_INT_2_10_10_10_REV;
            case ShaderDataType::Packed_2ZYX_Int:   return GL_INT_2_10_10_10_REV;
            }

            RW_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }

    public:
        OpenGLShader(const std::string& filepath, const std::string& name);

        OpenGLShader(const ShaderComponentPaths componentPaths, const std::string& name);

        ~OpenGLShader();

        void Bind() const override;

        void Unbind() const override;

        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& vector);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& vector);
        void UploadUniformInt(const std::string& name, const glm::int32 value);

        const std::string& GetName() const override { return m_Name; }

    private:
        void CompileAndLink(const std::string vertexCode, const std::string fragmentCode);
        static std::string ReadFile(const std::string& filepath);
        static int CompileShaderCode(const char* shaderCode, GLenum shaderType, char* infoLog);
        static void SplitSource(const std::string source, std::string& out_VertexCode, std::string& out_FragmentCode);


    private:
        uint32_t m_RendererID;

        const std::string m_Name;
    };
}
