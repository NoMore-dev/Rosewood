#pragma once

#include "Rosewood/Renderer/Texture/Texture.h"

namespace Rosewood
{
	struct ShadowMapSpecification
	{
		uint32_t Height = 0, Width = 0;
	};

	class ShadowMap
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void BindTexture(uint32_t slot) = 0;
		
		const ShadowMapSpecification& GetSpecification() const { return m_Specification; }

	public:
		static Ref<ShadowMap> Create(ShadowMapSpecification spec);

	protected:
		ShadowMapSpecification m_Specification = {};
	};
}