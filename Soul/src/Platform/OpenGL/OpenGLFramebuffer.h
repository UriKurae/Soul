#pragma once

#include "Soul/Renderer/Framebuffer.h"

namespace Soul
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void BindFBTexture() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { SL_CORE_ASSERT(index < colorAttachments.size(), "Color attachments size is less than index");  return colorAttachments[index]; }
		virtual uint32_t GetFrameBufferID() const override { return m_RendererID; }
	private:
		uint32_t m_RendererID = 0;
		std::vector<unsigned int> attachments = {};
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> colorAttachmentSpecs;
		FramebufferTextureSpecification depthAttachmentSpec = FramebufferTextureFormat::NONE;

		std::vector<uint32_t> colorAttachments = {};
		uint32_t depthAttachment = 0;
	};
}