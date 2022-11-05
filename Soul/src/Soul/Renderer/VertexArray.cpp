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
			case RendererAPI::None:		SL_CORE_ASSERT(false, "None is not supported"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		SL_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}