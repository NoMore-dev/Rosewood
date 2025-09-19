#pragma once

#include <glad/glad.h>

#include "Rosewood/Renderer/Shader/ShaderDataType.h"


namespace Rosewood
{
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

    static uint32_t OpenGLShaderDataTypeAlignement(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:             return 4;
            case ShaderDataType::Float2:            return 4 * 2;
            case ShaderDataType::Float3:            return 4 * 4;
            case ShaderDataType::Float4:            return 4 * 4;
            case ShaderDataType::Mat3:              return 4 * 4;
            case ShaderDataType::Mat4:              return 4 * 4;
            case ShaderDataType::Int:               return 4;
            case ShaderDataType::Int2:              return 4 * 2;
            case ShaderDataType::Int3:              return 4 * 4;
            case ShaderDataType::Int4:              return 4 * 4;
            case ShaderDataType::UShort2:           return 2;
            case ShaderDataType::Bool:              return 1;
            case ShaderDataType::Packed_2ZYX_UInt:  return 4;
            case ShaderDataType::Packed_2ZYX_Int:   return 4;
        }

        RW_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

}