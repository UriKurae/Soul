#include "slpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Soul
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}