#pragma once

#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/RenderCommand.h"
#include "Rosewood/Renderer/Texture/Texture.h"
#include <glm/vec4.hpp>

namespace Rosewood
{
    static uint32_t s_IDCounter = 0;

    struct MaterialSpecification
    {
        std::string ShaderName;
        CullMode CullMode   = CullMode::CullBack;
        Opacity Opacity     = Opacity::Opaque;
    };

    struct TextureBinding
    {
        TextureBindingSlot BindingSlot;
        Ref<Texture2D> Texture = nullptr;
    };


    class Material
    {
    public:
        Material(MaterialSpecification spec);
        ~Material() = default;

        template<typename T>
        inline void SetNamedParameter(const std::string& name, const T& value)
        {
            for (auto& element : m_Shader->GetMaterialDataLayout())
            {
                if (element.Name.compare(name) == 0)
                {
                    if (sizeof(T) != element.Size)
                    {
                        RW_WARN("Mismatching material parameter byte size and input value type size!");
                        return;
                    }

                    memcpy(m_MaterialData.data() + element.Offset, &value, element.Size);
                    return;
                }
            }

            RW_WARN("Material missing parameter name : {0}", name);
        }
        void SetTextureBinding(uint32_t slot, Ref<Texture2D> texture);

        void UploadMaterialData();

        const uint32_t GetID() const { return m_ID; }
        Ref<Shader> GetShader() { return m_Shader; }
        const MaterialSpecification& GetSpecification() const { return m_Spec; }
        std::vector<BYTE>& GetMaterialData() { return m_MaterialData; }
        const Ref<Texture2D> GetBindedTexture(uint32_t slot) const;

    private:
        uint32_t m_ID;

        MaterialSpecification m_Spec;

        Ref<Shader> m_Shader;
        std::vector<BYTE> m_MaterialData;
        std::vector<TextureBinding> m_TextureBindings;
    };
}