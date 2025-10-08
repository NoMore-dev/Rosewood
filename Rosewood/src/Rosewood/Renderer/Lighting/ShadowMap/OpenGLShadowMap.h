#pragma once

#include "ShadowMap.h"

namespace Rosewood
{
	class OpenGLShadowMap : public ShadowMap
	{
	public:
		OpenGLShadowMap(ShadowMapSpecification spec);
		~OpenGLShadowMap() = default;

		void Bind() override;
		void Unbind() override;
		void BindTexture(uint32_t slot) override;

	private:
		uint32_t m_FramebufferID = 0;
		uint32_t m_TextureID = 0;
	};
}