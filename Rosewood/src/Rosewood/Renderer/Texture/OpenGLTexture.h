#pragma once

#include "Rosewood/Renderer/Texture/Texture.h"

namespace Rosewood
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, const TextureSpecification& spec);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width;  };
		uint32_t GetHeight() const override { return m_Height;  };

		void Bind(uint32_t slot = 0) const;

	private:
		std::string m_Path;
		TextureSpecification m_Spec;

		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		
	};

}