#include "rwpch.h"
#include "scene.h"
#include "Rosewood/Renderer/Renderer.h"

namespace Rosewood
{
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(float dt)
	{
		
	}

	void Scene::Render()
	{
		Rosewood::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Rosewood::RenderCommand::Clear();

		Rosewood::Renderer::BeginScene(*m_Camera, m_Camera->GetTransform().GetMatrix());

		Rosewood::Renderer::PrepareShader(m_Shader);
		Rosewood::Renderer::Submit(m_Shader, m_VAO);

		Rosewood::Renderer::EndScene();
	}
}