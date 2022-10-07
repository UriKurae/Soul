#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern Soul::Application* Soul::CreateApplication();

int main(int argc, char** argv)
{
	Soul::Log::Init();
	SL_CORE_WARN("Initialized Log!");
	int a = 5;
	SL_INFO("Hello! Var={0}", a);

	auto app = Soul::CreateApplication();
	app->Run();
	delete app;
}

#endif