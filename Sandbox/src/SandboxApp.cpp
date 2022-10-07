#include <Soul.h>

class Sandbox : public Soul::Application
{
public:
	Sandbox()
	{

	} 

	~Sandbox()
	{

	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Sandbox();
}