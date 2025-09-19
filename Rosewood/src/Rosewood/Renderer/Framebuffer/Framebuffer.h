#pragma once

#include "Rosewood/Core.h"

namespace Rosewood
{
	enum AttachmentFormat
	{
		NO_ATTACHEMENT,

		RGBA,
		RED_INT,

		DEPTH_STENCIL,

		DEPTH_DEFAULT = DEPTH_STENCIL
	};

	struct FramebufferAttachmentsFormats
	{
		FramebufferAttachmentsFormats() = default;
		FramebufferAttachmentsFormats(const std::vector<AttachmentFormat>& colorFormats)
			: ColorFormats(colorFormats), DepthFormat(DEPTH_DEFAULT){}
		FramebufferAttachmentsFormats(const std::vector<AttachmentFormat>& colorFormats, AttachmentFormat depthFormat)
			: ColorFormats(colorFormats), DepthFormat(depthFormat){}

		std::vector<AttachmentFormat> ColorFormats;
		AttachmentFormat DepthFormat;
	};

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;
		FramebufferAttachmentsFormats AttachmentsFormats;
	};

	class Framebuffer
	{
	public:
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual void Resize(uint32_t newWidth, uint32_t newHeight) = 0;

		virtual void Rebuild() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentID(uint32_t attachement) const  = 0;

		virtual void BlitFrom(Ref<Framebuffer> sourceFramebuffer) = 0;

		static Ref<Framebuffer> Create(FramebufferSpecification& spec);
	};


}