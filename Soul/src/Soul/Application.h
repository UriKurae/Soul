#pragma once

#include "Core.h"

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

