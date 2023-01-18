#pragma once

#include <glm/glm.hpp>

#include "Soul/Core/Core.h"
#include "Soul/Core/KeyCodes.h"
#include "Soul/Core/MouseButtonCodes.h"

namespace Soul
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}