#pragma once

#include "Core.h"
#include "Soul/Events/Event.h"
#include "Soul/Events/ApplicationEvent.h"


#include "Window.h"

namespace Soul
{
	class SOUL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}

