#pragma once


#include "SceneHierarchy/SceneHierachy.h"

#include <Rosewood.h>

class Inspector
{
public:
	Inspector(std::shared_ptr<SceneHierachy> sceneHierarchy);
	~Inspector() = default;

	void OnImGuiRender();

private:
	void DrawTransformComponentInspector(Rosewood::TransformData& transformData);
	void DrawRenderableObject3DComponentInspector(Rosewood::RenderableObject3D& renderableObject);

private:
	Rosewood::Ref<Rosewood::Scene> m_Scene;
	std::shared_ptr<SceneHierachy> m_SceneHierarchy;
};