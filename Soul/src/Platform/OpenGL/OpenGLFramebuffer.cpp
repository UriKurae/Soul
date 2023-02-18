#include "slpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Soul
{
	static const uint32_t s_MaxFrameBufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec)
	{
		for (uint32_t i = 0; i < m_Specification.colorAttachments; ++i)
		{
			colorAttachments.push_back(i);
		}
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		for (int i = 0; i < colorAttachments.size(); ++i)
		{
			glDeleteTextures(1, &colorAttachments[i]);

		}
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			for (int i = 0; i < colorAttachments.size(); ++i)
			{
				glDeleteTextures(1, &colorAttachments[i]);
			}
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		
		
		for (uint32_t i = 0; i < m_Specification.colorAttachments; i++)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachments[i]);
			glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);

			if (!m_Specification.floatingPointFB)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i], 0);
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glDrawBuffers(2, attachments.data());

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		SL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.width, m_Specification.height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BindFBTexture() const
	{
		for (int i = 0; i < colorAttachments.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);
		}
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			SL_CORE_WARN("Attempted to resize framebuffer {0}, {1}", width, height);
			return;
		}
		m_Specification.width = width;
		m_Specification.height = height;

		Invalidate();
	}
}