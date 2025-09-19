#pragma once

#include "SceneHierarchy/SceneHierachy.h"

#include <Rosewood.h>

class SceneHierachyPanel
{
public:
	SceneHierachyPanel(std::shared_ptr<SceneHierachy> sceneHierarchy);
	~SceneHierachyPanel() = default;

	void OnImGuiRender();

private:
	Rosewood::Ref<Rosewood::Scene> m_Scene;
	std::shared_ptr<SceneHierachy> m_SceneHierarchy;
};