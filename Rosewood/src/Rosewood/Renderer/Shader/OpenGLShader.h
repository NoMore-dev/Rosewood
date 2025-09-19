#pragma once

#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Shader/OpenGLShaderDataType.h"

#include <glad/glad.h>

namespace Rosewood
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath, const std::string& name);

        OpenGLShader(const ShaderComponentPaths& componentPaths, const std::string& name);

        ~OpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        const std::string& GetName() const override { return m_Name; }

    private:    
        void ReflectShaderResources(const std::vector<uint32_t>& fragmentVulkanSpirv);

        void InitializeMaterialDataUniformBuffer();

        std::string ReadFile(const std::string& filepath);
        void SplitSource(const std::string& source, std::string& out_VertexSource, std::string& out_FragmentSource);

        void MakeVulkanSpirvFromVulkanSources(const std::string& vertexVulkanSource, const std::string& fragmentVulkanSource, std::vector<uint32_t>& out_vertexVulkanSpirv, std::vector<uint32_t>& out_fragmentVulkanSpirv);
        void MakeOpenGLSourcesFromVulkanSpirv(const std::vector<uint32_t>& vertexVulkanSpirv, const std::vector<uint32_t>& fragmentVulkanSpirv, std::string& out_vertexOpenGLSource, std::string& out_fragmentOpenGLSource);

        void CompileAndLinkFromOpenGLSources(const std::string& vertexOpenGLSource, const std::string& fragmentOpenGLSource);
        int CompileOpenGLSource(const char* shaderSource, GLenum shaderType, char* infoLog);


    private:
        uint32_t m_RendererID;
    };
}
 