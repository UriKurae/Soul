#pragma once

#include "Core.h"
#include "Soul/Events/Event.h"

namespace Soul
{
	class SOUL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}

