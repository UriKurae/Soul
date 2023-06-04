#pragma once

#include "Soul.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Soul/Renderer/EditorCamera.h"

#include "Platform/OpenGL/ComputeShader.h"

namespace Soul
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void ImportModel();
		void ExportImage(Ref<Texture2D> image);
	private:
		ShaderLibrary m_ShaderLibrary;
	
		std::vector<float> quadVerticesFB;
		Ref<VertexArray> vaoFB;
		Ref<VertexBuffer> vboFB;
		Ref<Shader> hdrShader;
		Ref<Shader> bloomShader;
		Ref<Texture2D> fBTexture;

		// Bloom
		int iterations = 5;
		float threshHold = 1.0f;
	
		Attachment currentAttachment;

		Ref<Framebuffer> textureFramebuffer;
		Ref<Framebuffer> hdrFramebuffer;

		std::vector<Ref<Framebuffer>> bloomBuffers;

		Ref<Scene> m_ActiveScene;
		
		EditorCamera m_EditorCamera;

		std::shared_ptr<ComputeShader> computeShader;
		Ref<Texture2D> computeShaderTexture;
		
		Entity m_CameraEntity;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0,0 };
		glm::vec2 viewportBounds[2];

		bool m_ViewportFocusedUVs = false;
		bool m_ViewportHoveredUVs = false;
		glm::vec2 m_ViewportSizeUVs = { 0,0 };
		glm::vec2 viewportBoundsUVs[2];

		bool holdingMouse = false;
		float offsetMouse =  0.0f;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, -0.8f };

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel contentBrowserPanel;
	};
}