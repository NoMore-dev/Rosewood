#pragma once

#ifdef RW_PLATFORM_WINDOWS

extern Rosewood::Application* Rosewood::CreateApplication();

int main(int argc, char** argv)
{
	Rosewood::Log::Init();
	RW_CORE_WARN("Initialized Log!");
	int a = 5;
	RW_INFO("Hello! Var = {0}",a);

	auto app = Rosewood::CreateApplication();
	app->Run();
	delete app;
}

#endif