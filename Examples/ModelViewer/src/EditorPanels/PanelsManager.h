#pragma once

#include "Rosewood.h"
#include "SceneHierarchy/SceneHierachy.h"
#include "EditorPanels/SceneHierachy/SceneHierachyPanel.h"
#include "EditorPanels/Inspector/Inspector.h"
#include "EditorPanels/MenuBar/MenuBar.h"
#include "EditorPanels/Viewport/ViewportPanel.h"

class PanelsManager
{
public:
	PanelsManager(std::shared_ptr<SceneHierachy> sceneHierarchy);
	~PanelsManager() = default;

	void OnStart();
	void OnUpdate(float dt);
	void OnImGuiRender();

	std::shared_ptr<ViewportPanel> GetViewportPanel() { return m_ViewportPanel; }

private:
	void SetStyle();

private:
	std::shared_ptr<SceneHierachy> m_SceneHierarchy;
	std::shared_ptr<SceneHierachyPanel> m_SceneHierachyPanel;
	std::shared_ptr<Inspector> m_InspectorPanel;
	std::shared_ptr<MenuBar> m_MenuBar;
	std::shared_ptr<ViewportPanel> m_ViewportPanel;

	float m_Deltatime = 0.f;
	int m_Fps = 0;
};