#pragma once

#include "Soul/Core/Core.h"

namespace Soul
{
	enum class Attachment
	{
		ALBEDO = 0,
		NORMALS,
		POSITION,
		BRIGHTNESS
	};
	enum class FramebufferTextureFormat
	{
		NONE = 0,

		// Color
		RGBA8,
		RED_INTEGER,
		RGBA16F,

		// Depth/Stencil
		DEPTH24STENCIL8,

		// Default
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
		: textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::NONE;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> fbAttachments)
			: attachments(fbAttachments) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t width;
		uint32_t height;
		FramebufferAttachmentSpecification attachments;
		uint32_t samples = 1;
		bool floatingPointFB = false;
		uint32_t colorAttachments = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:

		virtual ~Framebuffer() {}
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachments(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual void BindColorAttachmentTexture(uint32_t index, Attachment attach) const = 0;
		virtual uint32_t GetFrameBufferID() const = 0;

		virtual void BindFBTexture() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}