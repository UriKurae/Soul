#pragma once

#include "Soul/Core.h"

namespace Soul
{
	class SOUL_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPresset(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX(int button) { return s_Instance->GetMouseXImpl(button); }
		inline static float GetMouseY(int button) { return s_Instance->GetMouseYImpl(button); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl(int button) = 0;
		virtual float GetMouseYImpl(int button) = 0;
	private:
		static Input* s_Instance;
	};
}