#pragma once

#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Camera/PerspectiveCamera.h"

namespace Rosewood
{

	class Scene
	{
	public:
		Scene::Scene();
		Scene::~Scene();

		void Scene::OnUpdate(float dt);
		void Scene::Render();

	public:
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VAO;
		PerspectiveCamera* m_Camera;
	};

}