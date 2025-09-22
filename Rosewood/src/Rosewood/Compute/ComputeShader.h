#pragma once

#include <glm/vec3.hpp>

namespace Rosewood
{
    struct ComputeShaderSpecification
    {
        std::string Name;
        std::string Path;
        glm::ivec3 WorkGroupCount;
    };

    class ComputeShader
    {
    public:
        virtual ~ComputeShader() = default;

        static Ref<ComputeShader> Create(const ComputeShaderSpecification& shaderSpec);

        virtual void DispatchCompute() const = 0;
        virtual void SetBarrier() const = 0; //TEMPORARY

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const = 0;
        bool IsCompiled() const { return m_SuccessfullyCompiled; }

    protected:
        bool m_SuccessfullyCompiled = false;
        ComputeShaderSpecification m_Spec;
    };
}