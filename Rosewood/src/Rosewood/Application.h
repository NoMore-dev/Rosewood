#pragma once

#include "rwpch.h"
#include "Core.h"
#include "Events/Event.h"

namespace Rosewood
{

	class RW_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client.
	Application* CreateApplication();
}

