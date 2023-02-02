#include "slpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


namespace Soul
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		SL_CORE_ASSERT(false, "None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		SL_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SL_CORE_ASSERT(false, "None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		SL_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SL_CORE_ASSERT(false, "None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		SL_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}