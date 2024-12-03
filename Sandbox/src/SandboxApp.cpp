#include <Rosewood.h>
#include <imgui.h>

class MainLayer : public Rosewood::Layer
{
public:
	MainLayer()
		: Layer("Main"), m_Camera()
	{
		m_PrimaryScene = std::make_shared<Rosewood::Scene>();

		m_Camera.GetTransform().SetTranslation(glm::vec3(40.f, 0.f, 0.f));
		m_Camera.GetTransform().SetRotation(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));

		m_ShaderLibrary = Rosewood::ShaderLibrary();
		Rosewood::Ref<Rosewood::Shader> shader = m_ShaderLibrary.Load(Rosewood::ShaderComponentPaths{ "../Rosewood/shaders/simple.vert", "../Rosewood/shaders/flat.frag"}, "simple/flat");
		shader->Bind();
		shader->UploadUniformFloat4("u_Color", glm::vec4{ .21f, .2f, .2f, 1.f });

		Rosewood::MeshData mesh = Rosewood::MeshData("assets/meshes/weapon.dat", Rosewood::MeshData::FileFormat::CUSTOM);
		m_PrimaryScene->m_VAO = mesh.MakeVertexArray();
		m_PrimaryScene->m_Shader = shader;
		m_PrimaryScene->m_Camera = &m_Camera;

		//Rosewood::BufferLayout layout = {
		//	{ Rosewood::ShaderDataType::Float3,				"a_Position"		},
		//	{ Rosewood::ShaderDataType::Float3,				"a_Normal"			},
		//	{ Rosewood::ShaderDataType::UShort2,			"a_UV"				},
		//	{ Rosewood::ShaderDataType::Packed_2ZYX_UInt,	"a_Color",		true},
		//};

		//int a = 30;
		//std::vector<Rosewood::MeshData> batchedMeshes = std::vector<Rosewood::MeshData>(a * a * a);
		//std::vector<glm::mat4> batchTransforms = std::vector<glm::mat4>(a*a*a);
		//Rosewood::MeshData mesh = Rosewood::MeshData("assets/meshes/testCube.dat", Rosewood::MeshData::FileFormat::CUSTOM);
		//Rosewood::MeshData mesh2 = Rosewood::MeshData("assets/meshes/weapon.dat", Rosewood::MeshData::FileFormat::CUSTOM);
		//for (int x = 0; x < a; x++)
		//{
		//	for (int y = 0; y < a; y++)
		//	{
		//		for (int z = 0; z < a; z++)
		//		{
		//			if ((x * a * a + y * a + z) % 2) {
		//				batchedMeshes[x * a * a + y * a + z] = mesh;
		//			}
		//			else {
		//				batchedMeshes[x * a * a + y * a + z] = mesh2;
		//			}
		//			
		//			 
		//			glm::mat4 t = glm::mat4(1.0f);
		//			t = glm::scale(t, glm::vec3{  0.1f, 0.1f, 0.1f });
		//			t = glm::translate(t, glm::vec3{ x * -30.f, y * -30.f, z * -30.f });
		//			batchTransforms[x*a*a+y*a+z] = t;
		//		}
		//	}
		//}
		//Rosewood::Ref<Rosewood::Batch> batch = std::make_shared<Rosewood::Batch>(batchedMeshes, batchTransforms, layout);

		//m_VA = Rosewood::VertexArray::Create();
		//m_VA->Bind();

		//mesh = Rosewood::MeshData("assets/meshes/weapon.dat", Rosewood::MeshData::FileFormat::CUSTOM);
		//Rosewood::Ref<Rosewood::VertexBuffer> vb = Rosewood::VertexBuffer::Create((BYTE*)mesh.m_Vertices.data(), mesh.GetVertexCount() * sizeof(Rosewood::MeshData::Vertex));
		//vb->SetLayout(layout);
		//Rosewood::Ref<Rosewood::IndexBuffer> ib = Rosewood::IndexBuffer::Create(mesh.m_Indices.data(), mesh.GetIndexCount());

		//m_VA->AddVertexBuffer(vb);
		//m_VA->SetIndexBuffer(ib);
	}

	void OnUpdate(float dt) override
	{
		m_Deltatime = dt * 1000.f;
		m_Fps = (int)(1.f / dt);

		float speed = 20.0f;
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };

		if (Rosewood::Input::IsKeyPressed(RW_KEY_W))
			translation -= m_Camera.GetTransform().GetForward();
		if (Rosewood::Input::IsKeyPressed(RW_KEY_S))
			translation += m_Camera.GetTransform().GetForward();
		if (Rosewood::Input::IsKeyPressed(RW_KEY_SPACE))
			translation += m_Camera.GetTransform().GetUp();
		if (Rosewood::Input::IsKeyPressed(RW_KEY_LEFT_ALT))
			translation -= m_Camera.GetTransform().GetUp();
		if (Rosewood::Input::IsKeyPressed(RW_KEY_A))
			translation -= m_Camera.GetTransform().GetLeft();
		if (Rosewood::Input::IsKeyPressed(RW_KEY_D))
			translation += m_Camera.GetTransform().GetLeft();

		float length = glm::length(translation);
		if (length > 0.0f) translation /= length;

		m_Camera.GetTransform().SetTranslation(m_Camera.GetTransform().GetTranslation() + translation * speed * dt);


		auto [deltaHori, deltaVer] = Rosewood::Input::GetMousePositionDelta();
		float rotationSpeed = 0.002f;
		float yRotation = -deltaHori * rotationSpeed;
		float xRotation = -deltaVer * rotationSpeed;

		glm::vec3 targetRotation = m_Camera.GetTransform().GetRotation() + glm::vec3(xRotation, yRotation, 0.f) ;


		if (targetRotation.x > glm::pi<float>()/2.f) {
			targetRotation.x = glm::pi<float>()/2.f;
		}
		else if (targetRotation.x < -glm::pi<float>() / 2.f) {
			targetRotation.x = -glm::pi<float>() / 2.f;
		}
		 
		if (targetRotation.y > glm::pi<float>()) {
			targetRotation.y = -(2 * glm::pi<float>() - targetRotation.y);
		} 
		else if (targetRotation.y < -glm::pi<float>()) {
			targetRotation.y = 2 * glm::pi<float>() + targetRotation.y;
		}
		m_Camera.GetTransform().SetRotation(targetRotation);

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
	Rosewood::PerspectiveCamera m_Camera;

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