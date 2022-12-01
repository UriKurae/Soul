#pragma once

#include "Core.h"

#include "Window.h"
#include "Soul/Core/LayerStack.h"
#include "Soul/Events/Event.h"
#include "Soul/Events/ApplicationEvent.h"

#include "Soul/Core/Timestep.h"

#include "Soul/ImGui/ImGuiLayer.h"

namespace Soul
{
	class Application
	{
	public:
		Application(const std::string& name = "Soul App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}

