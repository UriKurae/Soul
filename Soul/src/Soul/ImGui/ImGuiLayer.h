#pragma once

#include "Soul/Layer.h"

#include "Soul/Events/ApplicationEvent.h"
#include "Soul/Events/KeyEvent.h"
#include "Soul/Events/MouseEvent.h"

namespace Soul
{
	class SOUL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}