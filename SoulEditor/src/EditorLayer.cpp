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

			Soul::FramebufferSpecification fbSpec;
			fbSpec.width = 1280;
			fbSpec.height = 720;
			m_Framebuffer = Framebuffer::Create(fbSpec);
		}

		void EditorLayer::OnDetach()
		{

		}

		void EditorLayer::OnUpdate(Soul::Timestep ts)
		{
			m_Framebuffer->Bind();
			Soul::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Soul::RenderCommand::Clear();

			Soul::Renderer::BeginScene();

			auto textureShader = m_ShaderLibrary.Get("Texture");

			m_Texture->Bind();
			Soul::Renderer::Submit(textureShader, m_SquareVA);

			m_ZoroTexture->Bind();
			Soul::Renderer::Submit(textureShader, m_SquareVA);

			Soul::Renderer::EndScene();
			m_Framebuffer->Unbind();
		}

		void EditorLayer::OnImGuiRender()
		{
			// Note: Switch this to true to enable dockspace
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

		
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
					if (ImGui::MenuItem("Exit")) Soul::Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");
			
			ImGui::Text("Renderer");

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("Viewport");

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
			{
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::PopStyleVar();

			ImGui::End();

			ImGui::End();
		}

		void EditorLayer::OnEvent(Event& event)
		{

		}
}