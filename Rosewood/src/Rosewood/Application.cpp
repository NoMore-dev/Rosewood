#include "rwpch.h"
#include "Application.h"
#include "Rosewood/Events/ApplicationEvent.h"
#include "Rosewood/Log.h"

namespace Rosewood
{

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run() 
	{
		WindowResizeEvent e(1280, 720);
		RW_TRACE(e.ToString());

		while (true);
	}

}