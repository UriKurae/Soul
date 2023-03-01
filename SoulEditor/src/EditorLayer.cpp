#include "EditorLayer.h"
#include <ImGui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Soul/Scene/SceneSerializer.h"

#include "Soul/Utils/PlatformUtils.h"

namespace Soul
{
	extern const std::filesystem::path assetsPath;

	EditorLayer::EditorLayer()
			: Layer("Editor")
	{
	}

		void EditorLayer::OnAttach()
		{
			computeShader = std::make_shared<ComputeShader>("assets/shaders/ComputeShader.glsl");
			computeShaderTexture = Texture2D::Create(512, 512);

			floatingFBShader = m_ShaderLibrary.Load("assets/shaders/HdrFrameBuffer.glsl");
			floatingFBShader->Bind();
			floatingFBShader->UploadUniformInt("hdrBuffer", 0);
			floatingFBShader->UploadUniformFloat("exposure", 25.0f);
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
			fbSpec.attachments = { FramebufferTextureFormat::RGBA8,FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			fbSpec.floatingPointFB = false;
			fbSpec.width = 1280;
			fbSpec.height = 720;
			m_Framebuffer = Framebuffer::Create(fbSpec);

			FramebufferSpecification hdrSpec;
			hdrSpec.attachments = { FramebufferTextureFormat::RGBA8,FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			hdrSpec.floatingPointFB = false;
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

			//// First pass with HDR framebuffer
			//hdrFramebuffer->Bind();
			//RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			//RenderCommand::Clear();
			//RenderCommand::ManageDepth(true);
			//
			//m_EditorCamera.OnUpdate(ts);

			//m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
		
			//

			//hdrFramebuffer->Unbind();

			///////////////////////////////// 

			//// Second pass with normal famebuffer
			//m_Framebuffer->Bind();
			//RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			//RenderCommand::Clear(true, false);
			//RenderCommand::ManageDepth(false);

			//hdrFramebuffer->BindFBTexture();
			//Renderer::SubmitArrays(floatingFBShader, vaoFB, 6);
			//
			//m_Framebuffer->Unbind();
			
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();


			RenderCommand::ManageDepth(true);
			m_Framebuffer->ClearAttachments(1, -1);
			
			m_EditorCamera.OnUpdate(ts);

			computeShaderTexture->BindToCompute();
			computeShader->Bind();
			m_ActiveScene->textureShader->Bind();
			computeShader->Dispatch();


			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);


			auto [mx, my] = ImGui::GetMousePos();
			mx -= viewportBounds[0].x;
			my -= viewportBounds[0].y;
			glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;
			
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				if (Input::IsMouseButtonPressed(Mouse::Button0))
				{					
					glm::vec3 hitPoint = {};
					glm::vec2 uvCoords = {};
					bool hit = false;
					if (m_ActiveScene->currentModel)
					{
						hit = m_EditorCamera.RayToMeshes(m_ActiveScene->currentModel, 100.0f, glm::vec2(mx, my), glm::vec2(viewportSize.x, viewportSize.y), hitPoint, uvCoords);
						if (hit)
						{
							m_ActiveScene->textureShader->Bind();
							m_ActiveScene->textureShader->UploadUniformFloat3("HitPoint", hitPoint);
							m_ActiveScene->textureShader->Unbind();
							m_ActiveScene->PaintModel(uvCoords);
						}
					}
				
				}

				//int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				//SL_CORE_WARN("Pixel data = {0}", pixelData);
			}

			m_Framebuffer->Unbind();

			// Control brush size shortcut
			if (Input::IsMouseButtonPressed(Mouse::Button0))
			{
				static float brushSize = 0.0f;
				if (!holdingMouse)
				{
					brushSize = m_ActiveScene->currentBrush.GetBrushSize();
					offsetMouse = ImGui::GetMousePos().x;
					holdingMouse = true;
				}
				
				if (Input::IsKeyPressed(Key::LeftControl))
				{
					float *currentBrush = &m_ActiveScene->currentBrush.GetBrushSize();
					float currentMousePos = ImGui::GetMousePos().x;
					*currentBrush = brushSize + currentMousePos - offsetMouse;
					if (*currentBrush < 0) *currentBrush = 0.0f;
					if (*currentBrush > 500.0f) *currentBrush = 500.0f;
				}
			}
			else
			{
				holdingMouse = false;
			}
			

			// For the UVs

			auto [mxUv, myUv] = ImGui::GetMousePos();
			mxUv -= viewportBoundsUVs[0].x;
			myUv -= viewportBoundsUVs[0].y;
			viewportSize = viewportBoundsUVs[1] - viewportBoundsUVs[0];
			myUv = viewportSize.y - myUv;
			mouseX = (int)mxUv;
			mouseY = (int)myUv;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				if (Input::IsMouseButtonPressed(Mouse::Button0))
				{
					m_ActiveScene->PaintModelUVs(glm::vec2(mouseX, mouseY), viewportSize);
				}
			}
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

				if (ImGui::BeginMenu("Edit"))
				{

					if (ImGui::BeginMenu("Lighting"))
					{
						if (ImGui::TreeNode("Exp level"))
						{
							static bool hdr = false;
							ImGui::Checkbox("Hdr?", &hdr);
							ImGui::DragFloat("Level", m_ActiveScene->GetSceneExposure(), 0.1f,0.1f, 100.0f, "%.3f");
							floatingFBShader->Bind();
							floatingFBShader->UploadUniformFloat("exposure", *m_ActiveScene->GetSceneExposure());
							floatingFBShader->UploadUniformInt("hdr", hdr);
							ImGui::TreePop();
						}
						
						ImGui::EndMenu();
					}
					
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			

			m_SceneHierarchyPanel.OnImGuiRender();
			contentBrowserPanel.OnImGuiRender();
			
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("Viewport");
	
			auto viewportOffset = ImGui::GetCursorPos();
			auto windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = ImGui::GetWindowPos();
			minBound.x += viewportOffset.x;
			minBound.y += viewportOffset.y;

			ImVec2 maxBound = { minBound.x + m_ViewportSize.x, minBound.y + m_ViewportSize.y };
			viewportBounds[0] = { minBound.x, minBound.y };
			viewportBounds[1] = { maxBound.x, maxBound.y };

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
			
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browser_Item"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					if (wcsstr(path, L"scenes") != 0)
					{
						OpenScene(std::filesystem::path(assetsPath) / path);
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::PopStyleVar();

			ImGui::End();

			int textureReady = m_ActiveScene->currentBrush.GetCurrentTextureID();
			if (textureReady != -1)
			{
				ImGui::Begin("UVs");


				auto viewportUVsOffset = ImGui::GetCursorPos();
				auto windowUVsSize = ImGui::GetWindowSize();
				ImVec2 minBoundUVs = ImGui::GetWindowPos();
				minBoundUVs.x += viewportUVsOffset.x;
				minBoundUVs.y += viewportUVsOffset.y;

				ImVec2 maxBoundUVs = { minBoundUVs.x + m_ViewportSizeUVs.x, minBoundUVs.y + m_ViewportSizeUVs.y };
				viewportBoundsUVs[0] = { minBoundUVs.x, minBoundUVs.y };
				viewportBoundsUVs[1] = { maxBoundUVs.x, maxBoundUVs.y };

				m_ViewportFocusedUVs = ImGui::IsWindowFocused();
				m_ViewportHoveredUVs = ImGui::IsWindowHovered();
				//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

				ImVec2 viewportPanelSizeUVs = ImGui::GetContentRegionAvail();
				if (m_ViewportSizeUVs != *((glm::vec2*)&viewportPanelSizeUVs) && viewportPanelSizeUVs.x > 0 && viewportPanelSizeUVs.y > 0)
				{
					//m_Framebuffer->Resize((uint32_t)viewportPanelSizeUVs.x, (uint32_t)viewportPanelSizeUVs.y);
					m_ViewportSizeUVs = { viewportPanelSizeUVs.x, viewportPanelSizeUVs.y };
				}

				ImGui::Image((void*)m_ActiveScene->currentBrush.GetCurrentTextureID(), ImVec2{ m_ViewportSizeUVs.x, m_ViewportSizeUVs.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				ImGui::End();

			}
			ImGui::End();
		}

		void EditorLayer::OnEvent(Event& event)
		{
			m_EditorCamera.OnEvent(event);

			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<KeyPressedEvent>(SL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
			dispatcher.Dispatch<MouseButtonPressedEvent>(SL_BIND_EVENT_FN(EditorLayer::OnMousePressed));
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

		bool EditorLayer::OnMousePressed(MouseButtonPressedEvent& e)
		{
			if (e.GetMouseButton() == Mouse::ButtonLeft)
			{
				
				return true;
			}
			return false;
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
				OpenScene(filePath);
			}
		}

		void EditorLayer::OpenScene(const std::filesystem::path& path)
		{
			m_ActiveScene = std::make_shared<Scene>();
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(path.string());
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