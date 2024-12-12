#include <Rosewood.h>
#include <imgui.h>

class MainLayer : public Rosewood::Layer
{
public:
	MainLayer()
		: Layer("Main")
	{
		m_PrimaryScene = std::make_shared<Rosewood::Scene>();
		m_ShaderLibrary = Rosewood::ShaderLibrary();

		// Meshes
		Rosewood::Ref<Rosewood::Shader> shader = m_ShaderLibrary.Load(Rosewood::ShaderComponentPaths{ "../Rosewood/shaders/simple.vert", "../Rosewood/shaders/flat.frag"}, "simple/flat");
		Rosewood::Ref<Rosewood::Material> material = std::make_shared<Rosewood::Material>(0, shader);
		Rosewood::Ref<Rosewood::Material> material2 = std::make_shared<Rosewood::Material>(1, shader);
		Rosewood::MeshData mesh = Rosewood::MeshData("assets/meshes/weapon.dat", Rosewood::MeshData::FileFormat::CUSTOM);
		for (int i = 0; i < 1; ++i) {
			Rosewood::EntityID entity = m_PrimaryScene->CreateEntity();
			glm::mat4 t = glm::mat4(1.0f);
			t = glm::translate(t, glm::vec3(0.f, (float)i * 3, 0.f));
			m_PrimaryScene->AddComponent<Rosewood::TransformComponent>(entity, t);
			m_PrimaryScene->AddComponent<Rosewood::MeshComponent>(entity, mesh.MakeVertexArray());

			if (i % 2)
				m_PrimaryScene->AddComponent<Rosewood::MaterialComponent>(entity, material);
			else
				m_PrimaryScene->AddComponent<Rosewood::MaterialComponent>(entity, material2);
		}

		//Camera
		Rosewood::TransformData cameraTransform = Rosewood::TransformData{};
		cameraTransform.SetTranslation(glm::vec3(40.f, 0.f, 0.f));
		cameraTransform.SetRotationEuler(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));
		Rosewood::Ref<Rosewood::PerspectiveCamera> camera = std::make_shared<Rosewood::PerspectiveCamera>();
		Rosewood::EntityID cameraEntityID = m_PrimaryScene->CreateEntity();
		m_PrimaryScene->AddComponent<Rosewood::TransformComponent>(cameraEntityID, cameraTransform);
		m_PrimaryScene->AddComponent<Rosewood::CameraComponent>(cameraEntityID, camera);
		m_PrimaryScene->SetPrimaryCameraID(cameraEntityID);

		//Lights
		Rosewood::EntityID pointLightEntity = m_PrimaryScene->CreateEntity();
		m_PrimaryScene->AddComponent<Rosewood::PointLightComponent>(pointLightEntity, glm::vec4(.1f), glm::vec3(.5f), glm::vec3(0.f), glm::vec3(1.f, .0003f, 0.0001f));
		Rosewood::TransformData pointLightTransform = Rosewood::TransformData(glm::vec3(10.f, 10.f, 0.f), glm::quat(), glm::vec3(1.f));
		m_PrimaryScene->AddComponent<Rosewood::TransformComponent>(pointLightEntity, pointLightTransform);
	}

	void OnUpdate(float dt) override
	{
		m_Deltatime = dt * 1000.f;
		m_Fps = (int)(1.f / dt);

		//Main Camera Controler
		{
			float speed = 20.0f;
			glm::vec3 translation = { 0.0f, 0.0f, 0.0f };

			if (m_PrimaryScene->HasPrimaryCamera())
			{
				Rosewood::EntityID cameraID = m_PrimaryScene->GetPrimaryCameraID();
				Rosewood::TransformData& cameraTransform = m_PrimaryScene->GetComponent<Rosewood::TransformComponent>(cameraID).Transform;

				if (Rosewood::Input::IsKeyPressed(RW_KEY_W))
					translation -= cameraTransform.GetForward();
				if (Rosewood::Input::IsKeyPressed(RW_KEY_S))
					translation += cameraTransform.GetForward();
				if (Rosewood::Input::IsKeyPressed(RW_KEY_SPACE))
					translation += cameraTransform.GetUp();
				if (Rosewood::Input::IsKeyPressed(RW_KEY_LEFT_ALT))
					translation -= cameraTransform.GetUp();
				if (Rosewood::Input::IsKeyPressed(RW_KEY_A))
					translation -= cameraTransform.GetLeft();
				if (Rosewood::Input::IsKeyPressed(RW_KEY_D))
					translation += cameraTransform.GetLeft();

				float length = glm::length(translation);
				if (length > 0.0f) translation /= length;

				cameraTransform.SetTranslation(cameraTransform.GetTranslation() + translation * speed * dt);

				auto [deltaHori, deltaVer] = Rosewood::Input::GetMousePositionDelta();
				float rotationSpeed = 0.002f;
				float yRotation = -deltaHori * rotationSpeed;
				float xRotation = -deltaVer * rotationSpeed;

				cameraTransform.RotateAround(xRotation, glm::vec3(1.f, 0.f, 0.f));
				cameraTransform.RotateAround(yRotation, glm::vec3(0.f, 1.f, 0.f));
			}
		}

		m_PrimaryScene->OnUpdate(dt);
		m_PrimaryScene->Render();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Frame Infos");
		ImGui::Text((std::to_string(m_Deltatime) + std::string(" ms")).c_str());
		ImGui::Text((std::to_string(m_Fps) + std::string(" fps")).c_str());
		ImGui::End();
	}


	void OnEvent(Rosewood::Event& event) override
	{

	}

	Rosewood::Ref<Rosewood::Scene> m_PrimaryScene;
	Rosewood::ShaderLibrary m_ShaderLibrary;

	int lastFrameMouseX = 0;
	int lastFrameMouseY = 0;

	float m_Deltatime = 0.f;
	int m_Fps = 0;
};

class Sandbox : public Rosewood::Application
{
public:
	Sandbox()
	{
		Rosewood::Window& window = GetWindow();
		window.DisableCursor();
		PushLayer(new MainLayer());
	}

	~Sandbox()
	{

	}

};

Rosewood::Application* Rosewood::CreateApplication() {
	return new Sandbox();
}