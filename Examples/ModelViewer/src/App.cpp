#include <Rosewood.h>
#include <Rosewood/Entrypoint.h>

#include <imgui.h>

#include "SceneHierarchy/SceneHierachy.h"
#include "EditorPanels/PanelsManager.h"

#include "CameraController/CameraController.h"
#include "Grid/Grid.h"


class MainLayer : public Rosewood::Layer
{
public:
	MainLayer()
		: Layer("Main")
	{
		Rosewood::ShaderLibrary::Load(Rosewood::ShaderSpecification{ "simple/flat",		"../../shaders/simple.vert",	"../../shaders/flat.frag"		});
		Rosewood::ShaderLibrary::Load(Rosewood::ShaderSpecification{ "simple/textured", "../../shaders/simple.vert",	"../../shaders/textured.frag"	});

		Rosewood::ComputeShaderSpecification computeSpec = Rosewood::ComputeShaderSpecification{};
			computeSpec.Name = "compute";
			computeSpec.Path = "../../shaders/Compute/compute.comp";
			computeSpec.WorkGroupCount = glm::ivec3(1);
		Rosewood::Ref<Rosewood::ComputeShader> computeShader = Rosewood::ComputeShader::Create(computeSpec);


		m_PrimaryScene = std::make_shared<Rosewood::Scene>();
		m_SceneHierarchy = std::make_shared<SceneHierachy>(m_PrimaryScene);

		m_PanelManager = std::make_unique<PanelsManager>(m_SceneHierarchy);

		m_CameraController = std::make_unique<CameraController>(m_PrimaryScene);
		m_Grid = std::make_unique<Grid>(m_PrimaryScene);
	}

	void OnAttach() override
	{
		m_CameraController->OnStart();
		m_PanelManager->OnStart();
	}
		
	void OnUpdate(float dt) override
	{
		m_CameraController->OnUpdate(dt);
		m_PrimaryScene->OnUpdate(dt);
		m_PanelManager->OnUpdate(dt);


		m_PanelManager->GetViewportPanel()->GetRenderFrameBuffer()->Bind();
		Rosewood::RenderCommand::SetClearColor({ 0.3f, 0.1f, 0.11f, 1.0f });
		Rosewood::RenderCommand::Clear();

		m_PrimaryScene->Render();

		m_PanelManager->GetViewportPanel()->GetRenderFrameBuffer()->Unbind();
	}


	void OnImGuiRender() override
	{
		m_PanelManager->OnImGuiRender();
	}


	void OnEvent(Rosewood::Event& event) override
	{
		m_CameraController->OnEvent(event);
	}

	std::shared_ptr<SceneHierachy> m_SceneHierarchy;
	std::unique_ptr<PanelsManager> m_PanelManager;

	std::unique_ptr<CameraController> m_CameraController;
	std::unique_ptr<Grid> m_Grid;

	Rosewood::Ref<Rosewood::Scene> m_PrimaryScene;
};


class App : public Rosewood::Application
{
public:
	App()
	{
		GetWindow().SetTitle("Model Viewer");
		GetWindow().SetVSync(false);
		PushLayer(new MainLayer());
	}

	~App()
	{

	}

};

Rosewood::Application* Rosewood::CreateApplication() {
	return new App();
}