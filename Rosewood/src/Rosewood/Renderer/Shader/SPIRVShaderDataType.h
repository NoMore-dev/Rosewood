#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <spirv_cross/spirv_cross.hpp>

#include "Rosewood/Renderer/Shader/ShaderDataType.h"

namespace Rosewood
{
    //TODO : support more types...
    static ShaderDataType SPIRTypeToShaderDataType(spirv_cross::SPIRType spirType)
    {
        switch (spirType.basetype)
        {
        case spirv_cross::SPIRType::BaseType::Float:
            switch (spirType.vecsize)
            {
            case 1:
                if (spirType.columns == 1)
                {
                    return ShaderDataType::Float;
                }
                break;
            case 2:
                if (spirType.columns == 1)
                {
                    return ShaderDataType::Float2;
                }
                break;
            case 3:
                if (spirType.columns == 1)
                {
                    return ShaderDataType::Float3;
                }
                if (spirType.columns == 3)
                {
                    return ShaderDataType::Mat3;
                }
                break;
            case 4:
                if (spirType.columns == 1)
                {
                    return ShaderDataType::Float4;
                }
                if (spirType.columns == 4)
                {
                    return ShaderDataType::Mat4;
                }
                break;
            }
        }

        RW_CORE_ASSERT(false, "This SpirType doesnt have a ShaderDataType equivalent yet!");
        return ShaderDataType::None;
    }
}