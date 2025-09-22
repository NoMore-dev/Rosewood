#pragma once

#include "Rosewood/Compute/ComputeShader.h"

#include <glad/glad.h>

namespace Rosewood
{
    class OpenGLComputeShader : public ComputeShader
    {
    public:
        OpenGLComputeShader(const ComputeShaderSpecification& shaderSpec);
        ~OpenGLComputeShader() = default;

        void DispatchCompute() const override;
        void SetBarrier() const override; //TEMPORARY

        void Bind() const override;
        void Unbind() const override;

        const std::string& GetName() const override { return m_Spec.Name; }

    private:
        void CompileAndLinkFromOpenGLSources(const std::string& source);
        int CompileOpenGLSource(const char* shaderSource, char* infoLog);

    private:
        uint32_t m_RendererID = 0;
    };
}