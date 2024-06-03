#include "rwpch.h"
#include "Application.h"
#include "Rosewood/Event/ApplicationEvent.h"
#include "Rosewood/Log/Log.h"
#include "Rosewood/Input/Input.h"

#include <filesystem>
#include <glad/glad.h>

namespace Rosewood
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr ;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Rosewood::ShaderDataType::Float:    return GL_FLOAT;
		case Rosewood::ShaderDataType::Float2:   return GL_FLOAT;
		case Rosewood::ShaderDataType::Float3:   return GL_FLOAT;
		case Rosewood::ShaderDataType::Float4:   return GL_FLOAT;
		case Rosewood::ShaderDataType::Mat3:     return GL_FLOAT;
		case Rosewood::ShaderDataType::Mat4:     return GL_FLOAT;
		case Rosewood::ShaderDataType::Int:      return GL_INT;
		case Rosewood::ShaderDataType::Int2:     return GL_INT;
		case Rosewood::ShaderDataType::Int3:     return GL_INT;
		case Rosewood::ShaderDataType::Int4:     return GL_INT;
		case Rosewood::ShaderDataType::Bool:     return GL_BOOL;
		}

		RW_CORE_ASSERT(false, "Unknown ShaderDataType!");
	}

	Application::Application()
	{
		RW_CORE_ASSERT(!s_Instance, "Application already exist !")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));


		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		uint32_t indices[3] = { 0, 1, 2 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		m_IndexBuffer->Bind();

		m_Shader.reset(Shader::Create("../Rosewood/shaders/shader.vert", "../Rosewood/shaders/shader.frag"));
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
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);

			m_Shader->Bind();

			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}