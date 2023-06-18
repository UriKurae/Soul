#include "EditorLayer.h"
#include <ImGui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Soul/Scene/SceneSerializer.h"

#include "Soul/Utils/PlatformUtils.h"
#include <Soul/Scene/Lights/DirectionalLight.h>
#include <Soul/Scene/Lights/Light.h>

namespace Soul
{
	extern const std::filesystem::path assetsPath;

	EditorLayer::EditorLayer()
			: Layer("Editor")
	{
	}

		void EditorLayer::OnAttach()
		{
			//computeShader = std::make_shared<ComputeShader>("assets/shaders/ComputeShader.glsl");
			//computeShaderTexture = Texture2D::Create(512, 512);
			
			bloomShader = m_ShaderLibrary.Load("assets/shaders/BloomShader.glsl");
			bloomShader->Bind();
			bloomShader->UploadUniformInt("image", 0);
			bloomShader->Unbind();

			currentAttachment = Attachment::ALBEDO;
			hdrShader = m_ShaderLibrary.Load("assets/shaders/HdrFrameBuffer.glsl");
			hdrShader->Bind();
			hdrShader->UploadUniformInt("hdrBuffer", 0);
			hdrShader->UploadUniformInt("blur", 1);
			hdrShader->UploadUniformFloat("exposure", 1.5f);
			hdrShader->UploadUniformInt("hdr", true);
			hdrShader->Unbind();

			

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

			FramebufferSpecification texturefbSpec;
			texturefbSpec.attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
			texturefbSpec.floatingPointFB = false;
			texturefbSpec.width = 1280;
			texturefbSpec.height = 720;
			textureFramebuffer = Framebuffer::Create(texturefbSpec);

			FramebufferSpecification hdrSpec;
			hdrSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
			hdrSpec.floatingPointFB = false;
			hdrSpec.width = 1280;
			hdrSpec.height = 720;
			hdrFramebuffer = Framebuffer::Create(hdrSpec);

			// Bloom buffers
			FramebufferSpecification bloom;
			bloom.attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
			bloom.floatingPointFB = false;
			bloom.width = 1280;
			bloom.height = 720;
			bloomBuffers.push_back(Framebuffer::Create(bloom));
			bloomBuffers.push_back(Framebuffer::Create(bloom));


			m_ActiveScene = std::make_shared<Scene>();
		
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);

			m_ActiveScene->SetMode(PaintMode::EDIT);
		}

		void EditorLayer::OnDetach()
		{

		}

		void EditorLayer::OnUpdate(Soul::Timestep ts)
		{
			// --------------------------- framebuffer resizing ---------------------------
			if (FramebufferSpecification spec = textureFramebuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
			{
				textureFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			}

			if (FramebufferSpecification spec = hdrFramebuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
			{
				hdrFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			}
			// --------------------------- framebuffer resizing end ---------------------------
			
			
			textureFramebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();


			RenderCommand::ManageDepth(true);
			
			m_EditorCamera.OnUpdate(ts);

			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);


			// --------------------------- Mouse related calculations ---------------------------
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
							m_ActiveScene->computeShader->Bind();
							m_ActiveScene->computeShader->UploadUniformFloat3("intersectionPoint", hitPoint);
							m_ActiveScene->computeShader->UploadUniformFloat2("uv", uvCoords);
							m_ActiveScene->computeShader->UploadUniformInt("painting", true);
							m_ActiveScene->currentBrush.painting = true;
						}
					}
				}
				else
				{
					m_ActiveScene->currentBrush.painting = false;
					m_ActiveScene->computeShader->UploadUniformInt("painting", false);
				}
			}

			Renderer::ClearTextures();
			textureFramebuffer->Unbind();

			// Blur excess of lighting
			bool horizontal = true, first_iteration = true;
			int amount = 10;
			bloomShader->Bind();

			for (unsigned int i = 0; i < amount; i++)
			{
				bloomBuffers[horizontal]->Bind();
				RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
				RenderCommand::Clear(true, false);
				RenderCommand::ManageDepth(false);
				bloomShader->UploadUniformInt("horizontal", horizontal);
				bloomShader->UploadUniformInt("iterations", iterations);
				if (first_iteration)
				{
					textureFramebuffer->BindColorAttachmentTexture(0, (Attachment)textureFramebuffer->GetColorAttachmentRendererID((uint32_t)Attachment::BRIGHTNESS));
				}
				else
				{
					bloomBuffers[!horizontal]->BindColorAttachmentTexture(0, (Attachment)bloomBuffers[!horizontal]->GetColorAttachmentRendererID(0));
				}
				vaoFB->Bind();

				Renderer::SubmitArrays(bloomShader, vaoFB, 6);
				Renderer::ClearTextures();
				
				horizontal = !horizontal;
				if (first_iteration)
					first_iteration = false;
			}
			bloomShader->Unbind();
			for (int i = 0; i < bloomBuffers.size(); ++i)
			{
				bloomBuffers[i]->Unbind();
			}
			// Blur end


			hdrFramebuffer->Bind();
			RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
			RenderCommand::Clear(true, false);
			RenderCommand::ManageDepth(false);


			hdrShader->Bind();

			hdrFramebuffer->BindColorAttachmentTexture(0, (Attachment)textureFramebuffer->GetColorAttachmentRendererID((uint32_t)currentAttachment));			
			hdrFramebuffer->BindColorAttachmentTexture(1, (Attachment)bloomBuffers[0]->GetColorAttachmentRendererID((uint32_t)Attachment::ALBEDO));
			vaoFB->Bind();
			
			Renderer::SubmitArrays(hdrShader, vaoFB, 6);
			Renderer::ClearTextures();
			
			hdrShader->Unbind();
			hdrFramebuffer->Unbind();

			// --------------------------- Shortcuts for artist ---------------------------
			if (Input::IsMouseButtonPressed(Mouse::Button0))
			{
				static float brushSize = 0.0f;
				static float lightDirCurrent = 0.0f;
				if (!holdingMouse)
				{
					lightDirCurrent = m_ActiveScene->RetieveMainLightTransform()->rotation.y;
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
				else if (Input::IsKeyPressed(Key::LeftShift))
				{
					auto light = m_ActiveScene->RetieveMainLightTransform();
					float currentMousePos = ImGui::GetMousePos().x - offsetMouse;
					light->rotation.y = glm::radians(lightDirCurrent + currentMousePos);
					
				}
			}
			else
			{
				holdingMouse = false;
			}
			// --------------------------- Shortcuts end ---------------------------

			// --------------------------- UV mouse calculations ---------------------------

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
			// --------------------------- Mouse related calculations End ---------------------------
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

					if (ImGui::MenuItem("Export Texture as png..."))
						ExportImage(m_ActiveScene->computeShaderTexture);

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
						SaveSceneAs();

					if (ImGui::MenuItem("Exit")) Soul::Application::Get().Close();
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Mode"))
				{
					const char* items[] = { "Edit Mode", "Paint Mode" };
					static int itemCurrent = 0;

					ImGui::Combo("##combo", &itemCurrent, items, IM_ARRAYSIZE(items));
					m_ActiveScene->SetMode((PaintMode)itemCurrent);

					ImGui::EndMenu();
				}


				if (ImGui::BeginMenu("Lighting"))
				{
					ImGui::Text("Exposure Level");
					ImGui::DragFloat("##Exposure Level", m_ActiveScene->GetSceneExposure(), 0.1f, 0.0f, 100.0f, "%.2f");

					static bool hdr = true;
					ImGui::Checkbox("Hdr?", &hdr);

					hdrShader->Bind();
					hdrShader->UploadUniformFloat("exposure", *m_ActiveScene->GetSceneExposure());
					hdrShader->UploadUniformInt("hdr", hdr);


					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Render Mode"))
				{
					const char* items[] = { "Albedo", "Normals" ,"Position", "Brightness" };
					static int itemCurrent = 0;

					ImGui::Combo("##combo", &itemCurrent, items, IM_ARRAYSIZE(items));
					currentAttachment = (Attachment)itemCurrent;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Skybox"))
				{
					const char* items[] = { "Field", "Yamakoto", "Sea", "Hotel Sea", "Basilica", "Italy"};
					static int currentSkybox = 0;

					if (ImGui::Combo("##combo", &currentSkybox, items, IM_ARRAYSIZE(items)))
					{
						m_ActiveScene->ChangeSkybox(currentSkybox);

					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Bloom"))
				{
					ImGui::Text("Iterations");
					ImGui::DragInt("##Iterations", &iterations, 1.0f, 0, 20);

					ImGui::Separator();
					ImGui::Text("Bloom Range");
					ImGui::DragFloat("##Bloom Range", &m_ActiveScene->threshHold, 0.1f, 0.0f, 100.0f, "%.2f");

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
				textureFramebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			}

			//uint32_t textureID = textureFramebuffer->GetColorAttachmentRendererID((uint32_t)currentAttachment);
			uint32_t textureID = hdrFramebuffer->GetColorAttachmentRendererID(0);
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
					//textureFramebuffer->Resize((uint32_t)viewportPanelSizeUVs.x, (uint32_t)viewportPanelSizeUVs.y);
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

		void EditorLayer::ExportImage(Ref<Texture2D> image)
		{
			std::string filepath = FileDialogs::SaveFile("Soul Texture (*.png)\0*.png\0");

			if (!filepath.empty())
			{
				if (image->GenerateImagePNG(filepath))
				{
					SL_CORE_INFO("Successfully generated PNG Image {0}", image->GetName());
				}
				else
				{
					SL_CORE_WARN("Error generating PNG Image {0}", image->GetName());
				}
			}
			
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