#pragma once

#include "SceneHierarchy/SceneHierachy.h"

#include <Rosewood.h>

class MenuBar
{
public:
	MenuBar(std::shared_ptr<SceneHierachy> sceneHierarchy);
	~MenuBar() = default;

	void OnImGuiRender();

private:
	Rosewood::Ref<Rosewood::Scene> m_Scene;
	std::shared_ptr<SceneHierachy> m_SceneHierarchy;

	Rosewood::Ref<Rosewood::Texture2D> m_CheckerBoardTexture;
};