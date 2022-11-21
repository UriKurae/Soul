#pragma once

#include "RenderCommand.h"
#include "Shader.h"

namespace Soul
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene();
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}