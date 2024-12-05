#include <Rosewood.h>
#include <imgui.h>

class MainLayer : public Rosewood::Layer
{
public:
	MainLayer()
		: Layer("Main")
	{
		m_ShaderLibrary = Rosewood::ShaderLibrary();
		Rosewood::Ref<Rosewood::Shader> shader = m_ShaderLibrary.Load(Rosewood::ShaderComponentPaths{ "../Rosewood/shaders/simple.vert", "../Rosewood/shaders/flat.frag"}, "simple/flat");
		shader->Bind();
		shader->UploadUniformFloat4("u_Color", glm::vec4{ .21f, .2f, .2f, 1.f });

		Rosewood::MeshData mesh = Rosewood::MeshData("assets/meshes/weapon.dat", Rosewood::MeshData::FileFormat::CUSTOM);

		m_PrimaryScene = std::make_shared<Rosewood::Scene>();

		Rosewood::TransformData cameraTransform = Rosewood::TransformData{};
		cameraTransform.SetTranslation(glm::vec3(40.f, 0.f, 0.f));
		cameraTransform.SetRotationEuler(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));

		Rosewood::Ref<Rosewood::PerspectiveCamera> camera = std::make_shared<Rosewood::PerspectiveCamera>();

		Rosewood::EntityID cameraEntityID = m_PrimaryScene->CreateEntity();
		m_PrimaryScene->AddComponent<Rosewood::TransformComponent>(cameraEntityID, cameraTransform);
		m_PrimaryScene->AddComponent<Rosewood::CameraComponent>(cameraEntityID, camera);
		m_PrimaryScene->SetPrimaryCameraID(cameraEntityID);

		for (int i = 0; i < 1; ++i) {
			Rosewood::EntityID entity = m_PrimaryScene->CreateEntity();
			glm::mat4 t = glm::mat4(1.0f);
			t = glm::translate(t, glm::vec3(0.f, (float)i * 3, 0.f));
			m_PrimaryScene->AddComponent<Rosewood::TransformComponent>(entity, t);
			m_PrimaryScene->AddComponent<Rosewood::MeshComponent>(entity, mesh.MakeVertexArray());
		}

		m_PrimaryScene->m_Shader = shader;
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