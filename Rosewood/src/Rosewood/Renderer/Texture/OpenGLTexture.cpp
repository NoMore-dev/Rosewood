#include "rwpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Rosewood
{
	static GLenum TextureFilteringGlenum(TextureFiltering filteringMode)
	{
		switch (filteringMode)
		{
		case Rosewood::Nearest:
			return GL_NEAREST;
		case Rosewood::Linear:
			return GL_LINEAR;
		default:
			break;
		}

		RW_ASSERT(false, "Texture filtering mode not supported !");
	}

	static GLenum TextureFilteringGlenum(TextureWrapping wrappingMode)
	{
		switch (wrappingMode)
		{
		case Rosewood::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		case Rosewood::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
		case Rosewood::Repeat:
			return GL_REPEAT;
		case Rosewood::Mirror:
			return GL_MIRRORED_REPEAT;
		default:
			break;
		}

		RW_ASSERT(false, "Texture wrapping mode not supported !");
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const TextureSpecification& spec)
		: m_Path(path), m_Spec(spec)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		RW_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 1) {
			internalFormat = GL_R8;
			dataFormat = GL_RED;
		}
		RW_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilteringGlenum(spec.MinFiltering));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilteringGlenum(spec.MagFiltering));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S,	 TextureFilteringGlenum(spec.HorizontalWrapping));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T,	 TextureFilteringGlenum(spec.VerticalWrapping));

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}