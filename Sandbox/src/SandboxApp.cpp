#include <Rosewood.h>

class ExampleLayer : public Rosewood::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Rosewood::Input::IsKeyPressed(RW_KEY_TAB))
			RW_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(Rosewood::Event& event) override
	{
		if (event.GetEventType() == Rosewood::EventType::KeyPressed)
		{
			Rosewood::KeyPressedEvent& e = (Rosewood::KeyPressedEvent&)event;
			if (e.GetKeyCode() == RW_KEY_TAB)
				RW_TRACE("Tab key is pressed (event)!");
			RW_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};

class Sandbox : public Rosewood::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Rosewood::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

Rosewood::Application* Rosewood::CreateApplication() {
	return new Sandbox();
}