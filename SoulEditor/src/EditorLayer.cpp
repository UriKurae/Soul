#include "EditorLayer.h"
#include <ImGui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Soul/Scene/SceneSerializer.h"

#include "Soul/Utils/PlatformUtils.h"

namespace Soul
{
	EditorLayer::EditorLayer()
			: Layer("Editor")
	{
	}

		void EditorLayer::OnAttach()
		{
			floatingFBShader = m_ShaderLibrary.Load("assets/shaders/HdrFrameBuffer.glsl");
			vaoFB = VertexArray::Create();
			vaoFB->Bind();
		
			quadVerticesFB = { 
				// positions   // texCoords
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			}; 
			vboFB = VertexBuffer::Create(quadVerticesFB.data(), sizeof(float) * quadVerticesFB.size());
			vboFB->SetLayout(
				{
					{ ShaderDataType::Float2, "a_Position" },
					{ ShaderDataType::Float2, "a_TexCoord" }
				});
			vaoFB->AddVertexBuffer(vboFB);

			FramebufferSpecification fbSpec;
			fbSpec.floatingPointFB = false;
			fbSpec.width = 1280;
			fbSpec.height = 720;
			m_Framebuffer = Framebuffer::Create(fbSpec);

			FramebufferSpecification hdrSpec;
			hdrSpec.floatingPointFB = true;
			hdrSpec.width = 1280;
			hdrSpec.height = 720;
			hdrFramebuffer = Framebuffer::Create(hdrSpec);

			m_ActiveScene = std::make_shared<Scene>();
					

			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
		}

		void EditorLayer::OnDetach()
		{

		}

		void EditorLayer::OnUpdate(Soul::Timestep ts)
		{
			if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
			{
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			}

			if (FramebufferSpecification spec = hdrFramebuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
			{
				hdrFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			}

			// First pass with HDR framebuffer
			hdrFramebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
			RenderCommand::ManageDepth(true);
			
			m_EditorCamera.OnUpdate(ts);

			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
		
			hdrFramebuffer->Unbind();

			/////////////////////////////// 

			// Second pass with normal famebuffer
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear(true, false);
			RenderCommand::ManageDepth(false);

			hdrFramebuffer->BindFBTexture();
			Renderer::SubmitArrays(floatingFBShader, vaoFB, 6);
			
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
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowMinSize.x = 190.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			style.WindowMinSize.x = 32.0f;
			
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New", "Ctrl+N"))
						NewScene();

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
						OpenScene();

					if (ImGui::MenuItem("Import Model...", "Ctrl+M"))
						ImportModel();

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
						SaveSceneAs();

					if (ImGui::MenuItem("Exit")) Soul::Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			

			m_SceneHierarchyPanel.OnImGuiRender();

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
			m_EditorCamera.OnEvent(event);

			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<KeyPressedEvent>(SL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		}

		bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
		{
			if (e.GetRepeatCount() > 0)
				return false;

			bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
			bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

			switch (e.GetKeyCode())
			{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();

				break;
			}

			}
			return true;
		}
		void EditorLayer::NewScene()
		{
			m_ActiveScene = std::make_shared<Scene>();
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		}

		void EditorLayer::OpenScene()
		{
			std::string filePath = FileDialogs::OpenFile("Soul Scene (*.soul)\0*.soul\0");
			if (!filePath.empty())
			{
				m_ActiveScene = std::make_shared<Scene>();
				m_SceneHierarchyPanel.SetContext(m_ActiveScene);


				SceneSerializer serializer(m_ActiveScene);
				serializer.Deserialize(filePath);
			}
		}

		void EditorLayer::SaveSceneAs()
		{
			std::string filepath = FileDialogs::SaveFile("Soul Scene (*.soul)\0*.soul\0");

			if (!filepath.empty())
			{
				SceneSerializer serializer(m_ActiveScene);
				serializer.Serialize(filepath);
			}
		}

		void EditorLayer::ImportModel()
		{
			std::string filePath = FileDialogs::OpenFile("Supported Files(*.fbx, *obj)\0*.fbx;*.obj;\0");
			if (!filePath.empty())
			{
				std::replace(filePath.begin(), filePath.end(), '\\', '/');
				m_ActiveScene->ImportModel(filePath);
			}
		}
}