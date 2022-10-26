#pragma once

#include "Soul/Input.h"

namespace Soul
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl(int button) override;
		virtual float GetMouseYImpl(int button) override;
	};
}