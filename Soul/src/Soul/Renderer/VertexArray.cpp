#include "slpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Soul
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		SL_CORE_ASSERT(false, "None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLVertexArray();
		}

		SL_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}