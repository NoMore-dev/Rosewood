#pragma once

#include "rwpch.h"
#include "Core.h"
#include "Rosewood/Layer/LayerStack.h"
#include "Event/Event.h"
#include "Rosewood/Window/Window.h"

namespace Rosewood
{

	class RW_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in client.
	Application* CreateApplication();
}

