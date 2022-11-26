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
		// Input Example for Soul
		/*if (Soul::Input::IsKeyPressed(SL_KEY_UP))
		{

		}*/
		m_Framebuffer->Bind();

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

		Soul::Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	virtual void OnImGuiRender() override
	{
		static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("Exit")) Soul::Application::Get().Close();
				ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
		ImGui::End();

		
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