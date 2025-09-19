#include <Rosewood.h>
#include <Rosewood/Entrypoint.h>

#include <imgui.h>

#include "SceneHierarchy/SceneHierachy.h"
#include "EditorPanels/SceneHierachy/SceneHierachyPanel.h"
#include "EditorPanels/Inspector/Inspector.h"
#include "EditorPanels/MenuBar/MenuBar.h"
#include "CameraController/CameraController.h"

class MainLayer : public Rosewood::Layer
{
public:
	MainLayer()
		: Layer("Main")
	{
		Rosewood::ShaderLibrary::Load(Rosewood::ShaderComponentPaths{ "../../shaders/simple.vert", "../../shaders/flat.frag" }, "simple/flat");
		Rosewood::ShaderLibrary::Load(Rosewood::ShaderComponentPaths{ "../../shaders/simple.vert", "../../shaders/textured.frag" }, "simple/textured");
		Rosewood::ShaderLibrary::Load(Rosewood::ShaderComponentPaths{ "../../shaders/Grid/grid.vert", "../../shaders/Grid/grid.frag" }, "grid");


		m_PrimaryScene = std::make_shared<Rosewood::Scene>();
		m_SceneHierachy = std::make_shared<SceneHierachy>(m_PrimaryScene);

		m_SceneHierachyPanel = std::make_unique<SceneHierachyPanel>(m_SceneHierachy);
		m_Inspector = std::make_unique<Inspector>(m_SceneHierachy);
		m_MenuBar = std::make_unique<MenuBar>(m_SceneHierachy);

		m_CameraController = std::make_unique<CameraController>(m_PrimaryScene);


		std::array<glm::vec3, 4>	vertices	= { glm::vec3(1., 0., 1.), glm::vec3(-1., 0., 1.), glm::vec3(-1., 0., -1.), glm::vec3(1., 0., -1.) };
		std::array<uint32_t, 6>		indices		= { 0, 3, 1, 3, 2, 1 };
		Rosewood::BufferLayout		layout		= { { Rosewood::ShaderDataType::Float3,				"a_Position"		} };

		Rosewood::Ref<Rosewood::VertexBuffer>	vb = Rosewood::VertexBuffer::Create((BYTE*)vertices.data(), layout.GetStride() * vertices.size(), layout);
		Rosewood::Ref<Rosewood::IndexBuffer>	ib = Rosewood::IndexBuffer::Create(indices.data(), indices.size());
		Rosewood::Ref<Rosewood::VertexArray>	va = Rosewood::VertexArray::Create();
		va->AddVertexBuffer(vb);
		va->SetIndexBuffer(ib);

		Rosewood::EntityID gridEntity = m_PrimaryScene->CreateEntity();
		m_PrimaryScene->AddComponent<Rosewood::TransformComponent>(gridEntity, Rosewood::TransformData(glm::vec3(0.f), glm::quat(), glm::vec3(1000.f)));

		Rosewood::MaterialSpecification materialSpec;
			materialSpec.ShaderName = "grid";
			materialSpec.CullMode	= Rosewood::CullMode::CullDisabled;
			materialSpec.Opacity	= Rosewood::Opacity::Transparent;
		Rosewood::Ref<Rosewood::Material> material = std::make_shared<Rosewood::Material>(materialSpec);
		std::vector<Rosewood::Ref<Rosewood::VertexArray>> vas = { va };
		std::vector<std::string> names = { "material" };
		std::vector<Rosewood::Ref<Rosewood::Material>> materials = { material };

		m_PrimaryScene->AddComponent<Rosewood::RenderableObject3D>(gridEntity, vas, materials, names);



		Rosewood::FramebufferSpecification fbSpec;
			fbSpec.Height = 1280;
			fbSpec.Width = 720;
			fbSpec.Samples = 1;
			fbSpec.AttachmentsFormats = Rosewood::FramebufferAttachmentsFormats{{Rosewood::AttachmentFormat::RGBA}, Rosewood::AttachmentFormat::NO_ATTACHEMENT};
		m_Framebuffer = Rosewood::Framebuffer::Create(fbSpec);

		Rosewood::FramebufferSpecification msfbSpec;
			msfbSpec.Height = 1280;
			msfbSpec.Width = 720;
			msfbSpec.Samples = 8;
			msfbSpec.AttachmentsFormats = Rosewood::FramebufferAttachmentsFormats{{Rosewood::AttachmentFormat::RGBA}};
		m_MultiSampledFramebuffer = Rosewood::Framebuffer::Create(msfbSpec);
	}

	void OnAttach() override
	{
		ImGui::StyleColorsLight();

		ImGuiStyle& style = ImGui::GetStyle();
			style.FrameBorderSize = 1.f;

			style.TabBarBorderSize = 1.f;
			style.TabBorderSize = 0.f;
			style.TabBarOverlineSize = 2.f;

			style.FrameRounding = 3.f;
			style.ChildRounding = 3.f;
			style.TabRounding = 3.f;
			style.WindowRounding = 3.f;

			style.DockingSeparatorSize = 1.f;


		m_CameraController->OnStart();
	}
		
	void OnUpdate(float dt) override
	{
		m_Deltatime = dt * 1000.f;
		m_Fps = (int)(1.f / dt);

		m_CameraController->OnUpdate(dt);
		m_PrimaryScene->OnUpdate(dt);


		m_MultiSampledFramebuffer->Bind();
		Rosewood::RenderCommand::SetClearColor({ 0.3f, 0.1f, 0.11f, 1.0f });
		Rosewood::RenderCommand::Clear();

		m_PrimaryScene->Render();

		m_MultiSampledFramebuffer->Unbind();

		m_Framebuffer->BlitFrom(m_MultiSampledFramebuffer);
	}


	void OnImGuiRender() override
	{
		// Dockspace
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 200.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			style.WindowMinSize.x = minWinSizeX;
		}


		m_MenuBar->OnImGuiRender();
		m_SceneHierachyPanel->OnImGuiRender();
		m_Inspector->OnImGuiRender();

		ImGui::Begin("Console");
		ImGui::End();

		ImGui::Begin("Frame Infos");
		ImGui::Text((std::to_string(m_Deltatime) + std::string(" ms")).c_str());
		ImGui::Text((std::to_string(m_Fps) + std::string(" fps")).c_str());
		ImGui::End();

		ImGui::Begin("Viewport");

		const Rosewood::PerspectiveCamera* const camera = (Rosewood::PerspectiveCamera*)m_PrimaryScene->GetComponent<Rosewood::CameraComponent>(m_PrimaryScene->GetPrimaryCameraID()).CameraRef.get();
		float aspectRatio = camera->GetSettings().ratio;

		ImVec2 viewportSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x * 1./aspectRatio);
		if (viewportSize.y > ImGui::GetContentRegionAvail().y)
			viewportSize = ImVec2(ImGui::GetContentRegionAvail().y * aspectRatio, ImGui::GetContentRegionAvail().y);

		uint32_t textureID = m_Framebuffer->GetColorAttachmentID(0);


		ImGuiStyle& style = ImGui::GetStyle();
		float size = viewportSize.x + style.FramePadding.x * 2.0f;
		float avail = ImGui::GetContentRegionAvail().x;
		float off = (avail - size) * 0.5f;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

		ImGui::Image((void*)textureID, viewportSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		m_Framebuffer->Resize(viewportSize.x, viewportSize.y);
		m_MultiSampledFramebuffer->Resize(viewportSize.x, viewportSize.y);

		ImGui::End();


		ImGui::End();
	}


	void OnEvent(Rosewood::Event& event) override
	{
		m_CameraController->OnEvent(event);
	}

	std::shared_ptr<SceneHierachy> m_SceneHierachy;
	std::unique_ptr<SceneHierachyPanel> m_SceneHierachyPanel;
	std::unique_ptr<Inspector> m_Inspector;
	std::unique_ptr<MenuBar> m_MenuBar;

	std::unique_ptr<CameraController> m_CameraController;

	Rosewood::Ref<Rosewood::Scene> m_PrimaryScene;
	Rosewood::Ref<Rosewood::Framebuffer> m_MultiSampledFramebuffer;
	Rosewood::Ref<Rosewood::Framebuffer> m_Framebuffer;
		
	float m_Deltatime = 0.f;
	int m_Fps = 0;

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