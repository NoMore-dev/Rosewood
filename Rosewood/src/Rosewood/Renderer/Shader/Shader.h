#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Rosewood
{
    enum class ShaderDataType : uint8_t
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, UShort2, Bool, Packed_2ZYX_UInt, Packed_2ZYX_Int
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:		        return 4;
        case ShaderDataType::Float2:	        return 8;
        case ShaderDataType::Float3:	        return 12;
        case ShaderDataType::Float4:	        return 16;
        case ShaderDataType::Mat3:		        return 36;
        case ShaderDataType::Mat4:		        return 64;
        case ShaderDataType::Int:		        return 4;
        case ShaderDataType::Int2:		        return 8;
        case ShaderDataType::Int3:		        return 12;
        case ShaderDataType::Int4:		        return 16;
        case ShaderDataType::UShort2:		    return 4;
        case ShaderDataType::Bool:		        return 1;
        case ShaderDataType::Packed_2ZYX_UInt:  return 4;
        case ShaderDataType::Packed_2ZYX_Int:   return 4;
        }

        RW_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    static uint32_t ShaderDataTypeComponentCount(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:             return 1;
        case ShaderDataType::Float2:            return 2;
        case ShaderDataType::Float3:            return 3;
        case ShaderDataType::Float4:            return 4;
        case ShaderDataType::Mat3:              return 9;
        case ShaderDataType::Mat4:              return 16;
        case ShaderDataType::Int:               return 1;
        case ShaderDataType::Int2:              return 2;
        case ShaderDataType::Int3:              return 3;
        case ShaderDataType::Int4:              return 4;
        case ShaderDataType::UShort2:		    return 2;
        case ShaderDataType::Bool:              return 1;
        case ShaderDataType::Packed_2ZYX_UInt:  return 4;
        case ShaderDataType::Packed_2ZYX_Int:   return 4;
        }

        RW_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct ShaderComponentPaths {
        std::string VertexPath;
        std::string FragmentPath;

        //Optional
        //std::string GeometryPath;
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        static Ref<Shader> Create(const std::string& filepath, const std::string& name );
        static Ref<Shader> Create(const ShaderComponentPaths componentPaths, const std::string& name);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
        virtual void UploadUniformFloat(const std::string& name, float value) = 0;
        virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& vector) = 0;
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& vector) = 0;
        virtual void UploadUniformInt(const std::string& name, const glm::int32 value) = 0;

        virtual const std::string& GetName() const = 0;

        bool IsCompiled() const { return m_SuccessfullyCompiled; }

    protected:
        const std::string m_Name;
        bool m_SuccessfullyCompiled = false;
    };

    class ShaderLibrary
    {
    public:
        Ref<Shader> Load(const std::string& filepath, const std::string& name);
        Ref<Shader> Load(const ShaderComponentPaths componentPaths, const std::string& name);

        Ref<Shader> Get(const std::string& name) const;

        int Add(const Ref<Shader> shader);

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders = std::unordered_map<std::string, Ref<Shader>>();
    };
}