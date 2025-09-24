#include "PanelsManager.h"
#include <imgui.h>

PanelsManager::PanelsManager(std::shared_ptr<SceneHierachy> sceneHierarchy)
	: m_SceneHierarchy(sceneHierarchy)
{
	m_SceneHierachyPanel = std::make_shared<SceneHierachyPanel>(m_SceneHierarchy);
	m_InspectorPanel = std::make_shared<Inspector>(m_SceneHierarchy);
	m_MenuBar = std::make_shared<MenuBar>(m_SceneHierarchy);
	m_ViewportPanel = std::make_shared<ViewportPanel>(m_SceneHierarchy->GetScene());

	SetStyle();
}

void PanelsManager::SetStyle()
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

}

void PanelsManager::OnStart()
{
}

void PanelsManager::OnUpdate(float dt)
{
	m_Deltatime = dt * 1000.f;
	m_Fps = (int)(1.f / dt);
}

void PanelsManager::OnImGuiRender()
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
	m_InspectorPanel->OnImGuiRender();
	m_ViewportPanel->OnImGuiRender();

	ImGui::Begin("Console");
	ImGui::End();

	ImGui::Begin("Frame Infos");
	ImGui::Text((std::to_string(m_Deltatime) + std::string(" ms")).c_str());
	ImGui::Text((std::to_string(m_Fps) + std::string(" fps")).c_str());
	ImGui::End();

	ImGui::End();
}
