#pragma once

#include "Rosewood/Renderer/Framebuffer/Framebuffer.h"

namespace Rosewood
{
	struct OpenGLAttachmentSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples;
		AttachmentFormat Format;
	};


	class OpenGLAttachment
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		uint32_t GetID() const { return m_RendererID; }
		uint32_t GetBindTarget() const { return m_BindTarget; }

	protected:
		uint32_t m_RendererID = 0;
		uint32_t m_BindTarget = 0;
		OpenGLAttachmentSpecification m_Specification = {};

		bool m_Valid = false;
	};

	class OpenGLTextureAttachment : public OpenGLAttachment
	{
	public:
		OpenGLTextureAttachment(OpenGLAttachmentSpecification spec);
		~OpenGLTextureAttachment();

		OpenGLTextureAttachment(OpenGLTextureAttachment&) = delete;
		OpenGLTextureAttachment(OpenGLTextureAttachment&& other);

		OpenGLTextureAttachment& operator=(const OpenGLTextureAttachment&) = delete;
		OpenGLTextureAttachment& operator=(const OpenGLTextureAttachment&&) = delete;

		void Bind() override;
		void Unbind() override;
	};

	class OpenGLRenderBufferAttachment : public OpenGLAttachment
	{
	public:
		OpenGLRenderBufferAttachment(OpenGLAttachmentSpecification spec);
		~OpenGLRenderBufferAttachment();

		OpenGLRenderBufferAttachment(OpenGLRenderBufferAttachment&) = delete;
		OpenGLRenderBufferAttachment(OpenGLRenderBufferAttachment&& other);

		OpenGLRenderBufferAttachment& operator=(const OpenGLRenderBufferAttachment&) = delete;
		OpenGLRenderBufferAttachment& operator=(const OpenGLRenderBufferAttachment&&) = delete;

		void Bind() override;
		void Unbind() override;

	};


	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Resize(uint32_t newWidth, uint32_t newHeight) override;
		void Rebuild() override;

		void Bind() override;
		void Unbind() override;

		const FramebufferSpecification& GetSpecification() const override;
		uint32_t GetColorAttachmentID(uint32_t attachement) const override;

		void BlitFrom(const Ref<Framebuffer> sourceFramebuffer) override;


	private:
		uint32_t GetId() const;

		void Actualise();

	private:
		FramebufferSpecification m_Specification;
		bool m_DirtyFlag = false;

		uint32_t m_RendererID = 0;

		std::vector<Ref<OpenGLAttachment>> m_ColorAttachments;
		Ref<OpenGLAttachment> m_DepthAttachment = nullptr;
	};


}