#include "slpch.h"
#include "Application.h"


#include "Soul/Events/ApplicationEvent.h"
#include "Soul/Log.h"

namespace Soul
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		SL_TRACE(e);

		while (true);
	}
}


