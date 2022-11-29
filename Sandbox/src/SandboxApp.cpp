#include <Soul.h>
#include "Soul/Core/EntryPoint.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Soul::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
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
	
	void OnAttach() override
	{
		m_Texture = Soul::Texture2D::Create("assets/textures/dog.jpg");
		m_ZoroTexture = Soul::Texture2D::Create("assets/textures/zoro.png");

		Soul::FramebufferSpecification fbSpec;

		fbSpec.width = 1280;
		fbSpec.height= 720;
		m_Framebuffer = Soul::Framebuffer::Create(fbSpec);
	}

	void OnDetach() override
	{

	}

	void OnUpdate(Soul::Timestep ts) override
	{
		m_Framebuffer->Bind();

		Soul::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Soul::RenderCommand::Clear();

		Soul::Renderer::BeginScene();
		
		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Soul::Renderer::Submit(textureShader, m_SquareVA);

		m_ZoroTexture->Bind();
		Soul::Renderer::Submit(textureShader, m_SquareVA);

		Soul::Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });

		ImGui::End();

	}

	void OnEvent(Soul::Event& event) override
	{

	}

private:
	Soul::ShaderLibrary m_ShaderLibrary;
	Soul::Ref<Soul::Shader> m_Shader;
	Soul::Ref<Soul::VertexArray> m_VertexArray;

	Soul::Ref<Soul::Shader> m_FlatColorShader;
	Soul::Ref<Soul::VertexArray> m_SquareVA;
	Soul::Ref<Soul::Framebuffer> m_Framebuffer;

	Soul::Ref<Soul::Texture2D> m_Texture, m_ZoroTexture;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, -0.8f };
};

class Sandbox : public Soul::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	} 

	~Sandbox()
	{

	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Sandbox();
}