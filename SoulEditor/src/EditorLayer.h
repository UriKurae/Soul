#pragma once

#include "Soul.h"

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
		ShaderLibrary m_ShaderLibrary;
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;

		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture, m_ZoroTexture;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0,0 };

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, -0.8f };

	};
}