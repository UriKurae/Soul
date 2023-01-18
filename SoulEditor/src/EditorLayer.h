#pragma once

#include "Soul.h"
#include "Panels/SceneHierarchyPanel.h"

#include "Soul/Renderer/EditorCamera.h"

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

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		ShaderLibrary m_ShaderLibrary;
		
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;

		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture, m_ZoroTexture;

		Ref<Scene> m_ActiveScene;
		
		EditorCamera m_EditorCamera;
		
		Entity m_CameraEntity;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0,0 };

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, -0.8f };

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}