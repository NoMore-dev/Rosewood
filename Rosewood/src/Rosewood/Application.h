#pragma once

#include "rwpch.h"
#include "Core.h"
#include "Rosewood/Layer/LayerStack.h"
#include "Event/Event.h"
#include "Rosewood/Window/Window.h"
#include "Rosewood/Event/ApplicationEvent.h"

#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Buffer/Buffer.h"
#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Camera/OrthographicCamera.h"

#include "Rosewood/ImGui/ImGuiLayer.h"

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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in client.
	Application* CreateApplication();
}

