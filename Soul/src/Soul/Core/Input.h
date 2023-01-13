#pragma once

#include "Soul/Core/Core.h"
#include "Soul/Core/KeyCodes.h"
#include "Soul/Core/MouseButtonCodes.h"

namespace Soul
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}