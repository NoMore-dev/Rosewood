#include "rwpch.h"
#include "Material.h"
#include "Rosewood/Utils/Conversions.h"

namespace Rosewood
{
	Material::Material(MaterialSpecification spec) : m_Spec(spec)
	{
		m_ID = s_IDCounter++;

		m_Shader = Rosewood::ShaderLibrary::Get(spec.ShaderName);
		m_MaterialData = std::vector<BYTE>(m_Shader->GetMaterialDataLayout().GetStride());

		for (auto& textureBindingSlot : m_Shader->GetTextureBindingSlots())
		{
			m_TextureBindings.push_back({ textureBindingSlot, nullptr });
		}
	}

	void Material::SetTextureBinding(uint32_t slot, Ref<Texture2D> texture)
	{
		for (auto& textureBinding : m_TextureBindings)
		{
			if (textureBinding.BindingSlot.Slot == slot)
			{
				textureBinding.Texture = texture;
				return;
			}		
		}
		
		RW_CORE_ASSERT(false, " Invalid texture slot! ");
	}

	void Material::UploadMaterialData()
	{
		Ref<UniformBuffer> materialDataUniformBuffer = m_Shader->GetMaterialDataUniformBuffer();
		materialDataUniformBuffer->SetData(m_MaterialData.data(), Utils::SizeToUint32(m_MaterialData.size()), 0);

		for (auto& textureBinding : m_TextureBindings)
		{
			if (textureBinding.Texture)
				textureBinding.Texture->Bind(textureBinding.BindingSlot.Slot);
		}
	}

	const Ref<Texture2D> Material::GetBindedTexture(uint32_t slot) const
	{
		for (auto& textureBinding : m_TextureBindings)
		{
			if (textureBinding.BindingSlot.Slot == slot)
				return textureBinding.Texture;
		}

		RW_CORE_ASSERT(false, " Invalid texture slot! ");
		return nullptr;
	}

}