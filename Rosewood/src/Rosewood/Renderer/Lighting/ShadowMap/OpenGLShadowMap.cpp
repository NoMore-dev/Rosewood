#include "rwpch.h"
#include "OpenGLShadowMap.h"
#include <glad/glad.h>

namespace Rosewood
{
	OpenGLShadowMap::OpenGLShadowMap(ShadowMapSpecification spec)
	{
		m_Specification = spec;

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, spec.Width, spec.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &m_FramebufferID);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TextureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLShadowMap::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	void OpenGLShadowMap::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
	}

	void OpenGLShadowMap::BindTexture(uint32_t slot)
	{
		glBindTextureUnit(slot, m_TextureID);
		
	}
}