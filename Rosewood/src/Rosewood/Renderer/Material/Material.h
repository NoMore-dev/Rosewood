#pragma once

#include "Rosewood/Renderer/Shader/Shader.h"
#include <glm/vec4.hpp>

namespace Rosewood
{
    class Material
    {
    public:
        Material(uint32_t ID, Ref<Shader> shader) : m_ID(ID), m_Shader(shader) {}
        ~Material() = default;

        const uint32_t GetID() const { return m_ID; }

        Ref<Shader> GetShader() { return m_Shader; }

        void SetColor(const glm::vec4& newColor) { m_Color = newColor; }
        const glm::vec4& GetColor() const { return m_Color; }

    private:
        uint32_t m_ID;
        Ref<Shader> m_Shader;
        glm::vec4 m_Color = glm::vec4(1.f);
    };
}