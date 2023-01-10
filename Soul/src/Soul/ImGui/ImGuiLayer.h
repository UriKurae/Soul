#pragma once

#include "Soul/Core/Layer.h"

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

		virtual void OnEvent(Event& e) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
		
		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}