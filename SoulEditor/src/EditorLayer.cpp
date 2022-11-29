#include "EditorLayer.h"
#include <ImGui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Soul
{
	EditorLayer::EditorLayer()
			: Layer("Editor")
	{
			m_SquareVA = Soul::VertexArray::Create();
			float squareVertices[5 * 4] =
			{
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
			};

			Soul::Ref<Soul::VertexBuffer> squareVB;
			squareVB = Soul::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
			squareVB->SetLayout({
				{ Soul::ShaderDataType::Float3, "a_Position" },
				{ Soul::ShaderDataType::Float2, "a_TexCoord" }
				});
			m_SquareVA->AddVertexBuffer(squareVB);

			uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
			Soul::Ref<Soul::IndexBuffer> squareIB;
			squareIB = Soul::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
			m_SquareVA->SetIndexBuffer(squareIB);

			auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");


			std::dynamic_pointer_cast<Soul::OpenGLShader>(textureShader)->Bind();
			std::dynamic_pointer_cast<Soul::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

		void EditorLayer::OnAttach()
		{
			m_Texture = Soul::Texture2D::Create("assets/textures/dog.jpg");
			m_ZoroTexture = Soul::Texture2D::Create("assets/textures/zoro.png");
		}

		void EditorLayer::OnDetach()
		{

		}

		void EditorLayer::OnUpdate(Soul::Timestep ts)
		{
			Soul::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Soul::RenderCommand::Clear();

			Soul::Renderer::BeginScene();

			auto textureShader = m_ShaderLibrary.Get("Texture");

			m_Texture->Bind();
			Soul::Renderer::Submit(textureShader, m_SquareVA);

			m_ZoroTexture->Bind();
			Soul::Renderer::Submit(textureShader, m_SquareVA);

			Soul::Renderer::EndScene();
		}

		void EditorLayer::OnImGuiRender()
		{
			ImGui::Begin("Settings");
			
			ImGui::Text("Renderer");

			ImGui::End();
		}

		void EditorLayer::OnEvent(Event& event)
		{

		}
}