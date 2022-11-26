#include "slpch.h"
#include "Framebuffer.h"

#include "Soul/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Soul
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		SL_CORE_ASSERT(false, "None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLFramebuffer>(spec);
		}

		SL_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}