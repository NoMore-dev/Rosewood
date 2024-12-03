#include "rwpch.h"
#include "Application.h"
#include "Rosewood/Event/ApplicationEvent.h"
#include "Rosewood/Log/Log.h"
#include "Rosewood/Input/Input.h"

#include "Rosewood/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Rosewood
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr ;

	Application::Application()
	{
		RW_CORE_ASSERT(!s_Instance, "Application already exist !")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(false);

		Input::Init();
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(RW_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(RW_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void Application::Run() 
	{	
		while (m_Running)
		{
			float time = m_Window->GetTime();
			float deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			Input::OnUpdate();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);
			
			m_ImGuiLayer->OnBegin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->OnEnd();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RenderCommand::SetViewportSize(e.GetWidth(), e.GetHeight());
		return true;
	}

}