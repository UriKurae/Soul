#include "slpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <Platform/OpenGL/OpenGLTexture.h>

namespace Soul
{
	
	std::unique_ptr<Renderer::SceneData> sceneData = std::make_unique<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

	}

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		sceneData->viewProjectionMatrix = camera.GetViewProjection();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::SubmitArrays(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, unsigned int indices)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", sceneData->viewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawWithArray(vertexArray, indices);
	}
}