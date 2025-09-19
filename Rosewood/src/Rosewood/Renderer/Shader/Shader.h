#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Rosewood/Renderer/Shader/ShaderDataType.h"
#include "Rosewood/Renderer/Buffer/Buffer.h"


namespace Rosewood
{

    struct ShaderComponentPaths {
        std::string VertexPath;
        std::string FragmentPath;
    };

    struct TextureBindingSlot
    {
        uint32_t Slot;
        std::string Name;
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        static Ref<Shader> Create(const std::string& filepath, const std::string& name );
        static Ref<Shader> Create(const ShaderComponentPaths& componentPaths, const std::string& name);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const = 0;
        bool IsCompiled() const { return m_SuccessfullyCompiled; }

        Ref<UniformBuffer> GetMaterialDataUniformBuffer() { return m_MaterialDataUniformBuffer; }
        const BufferLayout& GetMaterialDataLayout() const { return m_MaterialDataLayout; }
        const std::vector<TextureBindingSlot>& GetTextureBindingSlots() const { return m_TextureBindingSlots; };

    protected:
        bool m_SuccessfullyCompiled = false;
        std::string m_Name;
        
        Ref<UniformBuffer> m_MaterialDataUniformBuffer;

        BufferLayout m_MaterialDataLayout = {};
        std::vector<TextureBindingSlot> m_TextureBindingSlots;
    };

    class ShaderLibrary
    {
    public:
        ShaderLibrary() = delete;
        ~ShaderLibrary() = delete;


        static Ref<Shader> Load(const std::string& filepath, const std::string& name);
        static Ref<Shader> Load(const ShaderComponentPaths componentPaths, const std::string& name);

        static Ref<Shader> Get(const std::string& name);

        static int Add(const Ref<Shader> shader);

    private:
        static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
    };
}