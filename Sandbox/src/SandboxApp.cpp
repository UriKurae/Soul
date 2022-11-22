#include <Soul.h>

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
		m_VertexArray = Soul::VertexArray::Create();

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		Soul::Ref<Soul::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Soul::VertexBuffer::Create(vertices, sizeof(vertices)));
		Soul::BufferLayout layout = {
			{Soul::ShaderDataType::Float3, "a_Position"},
			{Soul::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Soul::Ref<Soul::IndexBuffer> indexBuffer;
		indexBuffer.reset(Soul::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Soul::VertexArray::Create();
		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Soul::Ref<Soul::VertexBuffer> squareVB;
		squareVB.reset(Soul::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Soul::ShaderDataType::Float3, "a_Position" },
			{ Soul::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Soul::Ref<Soul::IndexBuffer> squareIB;
		squareIB.reset((Soul::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;		
			out vec4 v_Color;		
	
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Soul::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			out vec3 v_Position;		
	
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;	
	
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = Soul::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Soul::Texture2D::Create("assets/textures/dog.jpg");
		m_ZoroTexture = Soul::Texture2D::Create("assets/textures/zoro.png");

		std::dynamic_pointer_cast<Soul::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Soul::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Soul::Timestep ts) override
	{
		// Input Example for Soul
		/*if (Soul::Input::IsKeyPressed(SL_KEY_UP))
		{

		}*/

		Soul::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Soul::RenderCommand::Clear();

		Soul::Renderer::BeginScene();
		
		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		// TODO: Carefull, to upload uniform first you have to bind
		//std::dynamic_pointer_cast<Soul::OpenGLShader>(m_FlatColorShader)->Bind();
		//std::dynamic_pointer_cast<Soul::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		//std::dynamic_pointer_cast<Soul::OpenGLShader>(m_TextureShader)->Bind();

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Soul::Renderer::Submit(textureShader, m_SquareVA);

		m_ZoroTexture->Bind();
		Soul::Renderer::Submit(textureShader, m_SquareVA);


		//std::dynamic_pointer_cast<Soul::OpenGLShader>(m_Shader)->Bind();
		//Soul::Renderer::Submit(m_Shader, m_VertexArray);

		Soul::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
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