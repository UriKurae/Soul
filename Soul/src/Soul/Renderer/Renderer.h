#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Soul/Renderer/Camera.h"
#include "Soul/Renderer/EditorCamera.h"

namespace Soul
{
	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform);
		static void SubmitArrays(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, unsigned int indices);

		static void ClearTextures();
		static void ManageDepth(bool enable);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		};
	};
}