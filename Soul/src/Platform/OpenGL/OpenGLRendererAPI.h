#pragma once

#include "Soul/Renderer/RendererAPI.h"

namespace Soul
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear(bool color, bool depth) override;
		virtual void ClearTextures() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawArrays(const Ref<VertexArray>& vertexArray, unsigned int indices) override;
		virtual void ManageDepth(bool enable);
	};
}