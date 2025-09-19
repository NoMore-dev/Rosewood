#include "rwpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Rosewood
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		RW_ASSERT(spec.AttachmentsFormats.ColorFormats.size() <= 8, "Failed to create framebuffer : Too many color attachments!");
		Rebuild();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const
	{
		return m_Specification;
	}

	void OpenGLFramebuffer::Resize(uint32_t newWidth, uint32_t newHeight)
	{
		if (m_Specification.Width == newWidth && m_Specification.Height == newHeight)
			return;


		m_Specification.Width = newWidth;
		m_Specification.Height = newHeight;

		m_DirtyFlag = true;
	}

	void OpenGLFramebuffer::Rebuild()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
		}
		m_ColorAttachments.clear();
		m_DepthAttachment = nullptr;


		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		

		GLenum color_attachement_index = GL_COLOR_ATTACHMENT0;
		for (AttachmentFormat format : m_Specification.AttachmentsFormats.ColorFormats)
		{
			Ref<OpenGLAttachment> attachement = nullptr;

			if (format != AttachmentFormat::NO_ATTACHEMENT)
			{
				if (m_Specification.Samples > 1)
				{
					attachement = std::make_shared<OpenGLRenderBufferAttachment>(OpenGLRenderBufferAttachment(OpenGLAttachmentSpecification{ m_Specification.Width, m_Specification.Height, m_Specification.Samples, format }));
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, color_attachement_index, attachement->GetBindTarget(), attachement->GetID());
				}
				else
				{
					attachement = std::make_shared<OpenGLTextureAttachment>(OpenGLTextureAttachment(OpenGLAttachmentSpecification{ m_Specification.Width, m_Specification.Height, 1, format }));
					glFramebufferTexture2D(GL_FRAMEBUFFER, color_attachement_index, attachement->GetBindTarget(), attachement->GetID(), 0);

				}
			}

			m_ColorAttachments.push_back(attachement);
			color_attachement_index++;
		}

		if (m_Specification.AttachmentsFormats.DepthFormat != AttachmentFormat::NO_ATTACHEMENT)
		{
			AttachmentFormat format = m_Specification.AttachmentsFormats.DepthFormat;
			m_DepthAttachment = std::make_shared<OpenGLRenderBufferAttachment>(OpenGLRenderBufferAttachment(OpenGLAttachmentSpecification{ m_Specification.Width, m_Specification.Height, m_Specification.Samples, format }));
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment->GetBindTarget(), m_DepthAttachment->GetID());
		}


		RW_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_DirtyFlag = false;
	}

	void OpenGLFramebuffer::Bind()
	{
		Actualise();

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t OpenGLFramebuffer::GetColorAttachmentID(uint32_t attachement) const
	{
		RW_ASSERT( (m_ColorAttachments.size() > attachement  &&  m_ColorAttachments[attachement] != nullptr) , "Failed to query color attachment ID : Framebuffer missing color attachement {0}!", attachement);
		return m_ColorAttachments[attachement]->GetID();
	}

	void OpenGLFramebuffer::BlitFrom(const Ref<Framebuffer> sourceFramebuffer)
	{
		const FramebufferSpecification srcSpec = sourceFramebuffer->GetSpecification();
		uint32_t srcId = ((OpenGLFramebuffer*)sourceFramebuffer.get())->GetId();

		Actualise();
		((OpenGLFramebuffer*)sourceFramebuffer.get())->Actualise();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, srcId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
		glBlitFramebuffer(0, 0, srcSpec.Width, srcSpec.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	uint32_t OpenGLFramebuffer::GetId() const
	{
		return m_RendererID;
	}

	void OpenGLFramebuffer::Actualise()
	{
		if (m_DirtyFlag)
			Rebuild();
	}



	OpenGLTextureAttachment::OpenGLTextureAttachment(OpenGLAttachmentSpecification spec)
	{
		m_Specification = spec;

		if (spec.Samples > 1)
		{
			m_BindTarget = GL_TEXTURE_2D_MULTISAMPLE;

			glCreateTextures(m_BindTarget, 1, &m_RendererID);
			glBindTexture(m_BindTarget, m_RendererID);

			switch (spec.Format)
			{
				case RGBA:
					glTexImage2DMultisample(m_BindTarget, spec.Samples, GL_RGBA8, spec.Width, spec.Height, GL_FALSE);
					break;
				case RED_INT:
					glTexImage2DMultisample(m_BindTarget, spec.Samples, GL_RED_INTEGER, spec.Width, spec.Height, GL_FALSE);
					break;
				case DEPTH_STENCIL:
					glTexImage2DMultisample(m_BindTarget, spec.Samples, GL_DEPTH24_STENCIL8, spec.Width, spec.Height, GL_FALSE);
					break;
				default:
					RW_ASSERT(false, "Unsupported attachment format!");
			}
		}
		else
		{
			m_BindTarget = GL_TEXTURE_2D;

			glCreateTextures(m_BindTarget, 1, &m_RendererID);
			glBindTexture(m_BindTarget, m_RendererID);

			switch (spec.Format)
			{
				case RGBA:
					glTexImage2D(m_BindTarget, 0, GL_RGBA8, spec.Width, spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					glTexParameteri(m_BindTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(m_BindTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
				case RED_INT:
					glTexImage2D(m_BindTarget, 0, GL_RED_INTEGER, spec.Width, spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					glTexParameteri(m_BindTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(m_BindTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
				case DEPTH_STENCIL:
					glTexImage2D(m_BindTarget, 0, GL_DEPTH24_STENCIL8, spec.Width, spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
					glTexParameteri(m_BindTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(m_BindTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					break;
				default:
					RW_ASSERT(false, "Unsupported attachment format!");
			}
		}
			
		glBindTexture(m_BindTarget, 0);

		m_Valid = true;
	}

	OpenGLTextureAttachment::OpenGLTextureAttachment(OpenGLTextureAttachment&& other)
	{
		other.m_Valid = false;
		m_RendererID = other.m_RendererID;
		m_BindTarget = other.m_BindTarget;
		m_Specification = other.m_Specification;
		m_Valid = true;
	}

	OpenGLTextureAttachment::~OpenGLTextureAttachment()
	{
		if (m_Valid)
			glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureAttachment::Bind()
	{
		glBindTexture(m_BindTarget, m_RendererID);
	}

	void OpenGLTextureAttachment::Unbind()
	{
		glBindTexture(m_BindTarget, 0);
	}


	OpenGLRenderBufferAttachment::OpenGLRenderBufferAttachment(OpenGLAttachmentSpecification spec)
	{
		m_Specification = spec;
		m_BindTarget = GL_RENDERBUFFER;

		GLenum colorFormat = 0;
		switch (spec.Format)
		{
			case RGBA:
				colorFormat = GL_RGBA8; break;
			case RED_INT:
				colorFormat = GL_RED_INTEGER; break;
			case DEPTH_STENCIL:
				colorFormat = GL_DEPTH24_STENCIL8; break;
			default:
				RW_ASSERT(false, "Unsupported attachment format!");
		}

		glCreateRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(m_BindTarget, m_RendererID);
		if (spec.Samples > 1)
			glRenderbufferStorageMultisample(m_BindTarget, spec.Samples, colorFormat, spec.Width, spec.Height);
		else
			glRenderbufferStorage(m_BindTarget, colorFormat, spec.Width, spec.Height);

		glBindRenderbuffer(m_BindTarget, 0);

		m_Valid = true;
	}

	OpenGLRenderBufferAttachment::OpenGLRenderBufferAttachment(OpenGLRenderBufferAttachment&& other)
	{
		other.m_Valid = false;
		m_RendererID = other.m_RendererID;
		m_BindTarget = other.m_BindTarget;
		m_Specification = other.m_Specification;
		m_Valid = true;
	}

	OpenGLRenderBufferAttachment::~OpenGLRenderBufferAttachment()
	{
		if (m_Valid)
			glDeleteRenderbuffers(1, &m_RendererID);
	}

	void OpenGLRenderBufferAttachment::Bind()
	{
		glBindRenderbuffer(m_BindTarget, m_RendererID);
	}

	void OpenGLRenderBufferAttachment::Unbind()
	{
		glBindRenderbuffer(m_BindTarget, 0);
	}
}

