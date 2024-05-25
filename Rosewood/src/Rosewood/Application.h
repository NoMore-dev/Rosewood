#pragma once

#include "rwpch.h"
#include "Core.h"
#include "Events/Event.h"
#include "Rosewood/Window/Window.h"

namespace Rosewood
{

	class RW_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in client.
	Application* CreateApplication();
}

