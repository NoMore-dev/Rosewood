#pragma once

#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Shader/OpenGLShaderDataType.h"

#include <glad/glad.h>

namespace Rosewood
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const ShaderSpecification& shaderSpec);

        ~OpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        const std::string& GetName() const override { return m_Name; }

    private:    
        void ReflectShaderResources(const std::vector<uint32_t>& fragmentVulkanSpirv);

        void InitializeMaterialDataUniformBuffer();

        void MakeVulkanSpirvFromVulkanSource(ShaderStage stage, const std::string& vulkanSource, std::vector<uint32_t>& out_VulkanSpirv);
        void MakeOpenGLSourcesFromVulkanSpirv(ShaderStage stage, const std::vector<uint32_t>& vulkanSpirv, std::string& out_OpenGLSource);

        void CompileAndLinkFromOpenGLSources(const std::vector<std::pair<ShaderStage, std::string>>& sources);
        int CompileOpenGLSource(const char* shaderSource, GLenum shaderType, char* infoLog);


    private:
        uint32_t m_RendererID;
    };
}
 