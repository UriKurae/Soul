#pragma once

#include "Soul/Core/Core.h"
#include "Soul/Core/KeyCodes.h"
#include "Soul/Core/MouseButtonCodes.h"

namespace Soul
{
	class Input
	{
	public:
		inline static bool IsKeyPressed(int keycode);

		inline static bool IsMouseButtonPressed(int button);
		inline static std::pair<float, float> GetMousePosition();
		inline static float GetMouseX();
		inline static float GetMouseY();
	};
}