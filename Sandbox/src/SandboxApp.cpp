#include <Soul.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Soul::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{		
	}

	void OnUpdate() override
	{
		if (Soul::Input::IsKeyPressed(SL_KEY_TAB))
		{
			SL_INFO("Tab pressed");
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::End();
	}

	void OnEvent(Soul::Event& event) override
	{
	}


};

class Sandbox : public Soul::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	} 

	~Sandbox()
	{

	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Sandbox();
}