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
}