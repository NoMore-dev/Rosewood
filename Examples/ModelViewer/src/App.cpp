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


		//Lights
		Rosewood::EntityID directionalLightEntity = m_PrimaryScene->GetRegistry()->CreateEntity();
		m_SceneHierarchy->AddEntityToHierachy(directionalLightEntity);

		//m_PrimaryScene->GetRegistry()->AddComponent<Rosewood::TagComponent>(pointLightEntity, "PointLight");
		//m_PrimaryScene->GetRegistry()->AddComponent<Rosewood::PointLightComponent>(pointLightEntity, glm::vec4(.1f), glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.f, .0003f, 0.0001f));
		//Rosewood::TransformData pointLightTransform = Rosewood::TransformData(glm::vec3(30.f, 20.f, 0.f), glm::quat(), glm::vec3(1.f));
		//m_PrimaryScene->GetRegistry()->AddComponent<Rosewood::TransformComponent>(pointLightEntity, pointLightTransform);

		m_PrimaryScene->GetRegistry()->AddComponent<Rosewood::TagComponent>(directionalLightEntity, "Directional Light");
		m_PrimaryScene->GetRegistry()->AddComponent<Rosewood::DirectionalLightComponent>(directionalLightEntity, glm::vec4(.2f), glm::vec3(.5f), glm::vec3(.5f));
		Rosewood::TransformData pointLightTransform = Rosewood::TransformData(glm::vec3(30.f, 20.f, 0.f), glm::quat(), glm::vec3(1.f));
		pointLightTransform.SetRotationEuler(glm::vec3((glm::pi<float>() / 4.f), -glm::pi<float>() / 2.f, 0.f));
		m_PrimaryScene->GetRegistry()->AddComponent<Rosewood::TransformComponent>(directionalLightEntity, pointLightTransform);
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

		m_PrimaryScene->Render();
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