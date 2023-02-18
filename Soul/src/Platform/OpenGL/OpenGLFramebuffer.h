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
		virtual uint32_t GetColorAttachmentRendererID() const override { return colorAttachments[0]; }
		virtual uint32_t GetFrameBufferID() const override { return m_RendererID; }
	private:
		uint32_t m_RendererID = 0;
		std::vector<uint32_t> colorAttachments = {};
		std::vector<unsigned int> attachments = {};
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};
}