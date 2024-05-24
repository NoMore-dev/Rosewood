#pragma once

#ifdef RW_PLATFORM_WINDOWS

extern Rosewood::Application* Rosewood::CreateApplication();

int main(int argc, char** argv)
{
	printf("Rosewood is running !\n");
	auto app = Rosewood::CreateApplication();
	app->Run();
	delete app;
}

#endif