#include <Soul.h>

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

	void OnEvent(Soul::Event& event) override
	{
		SL_TRACE("{0}", event);
	}

};

class Sandbox : public Soul::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Soul::ImGuiLayer());
	} 

	~Sandbox()
	{

	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Sandbox();
}