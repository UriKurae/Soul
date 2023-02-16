#pragma once

#include "RendererAPI.h"

namespace Soul
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear(bool color = true, bool depth = true)
		{
			s_RendererAPI->Clear(color, depth);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawWithArray(const Ref<VertexArray>& vertexArray, unsigned int indices)
		{
			s_RendererAPI->DrawArrays(vertexArray, indices);
		}

		inline static void ManageDepth(bool enable)
		{
			s_RendererAPI->ManageDepth(enable);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}