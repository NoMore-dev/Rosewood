#include "rwpch.h"
#include "scene.h"
#include "Rosewood/Renderer/Renderer.h"
#include "Rosewood/Scene/Components.h"

namespace Rosewood
{
	void Scene::OnUpdate(float dt)
	{
	}

	void Scene::Render()
	{
		Rosewood::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Rosewood::RenderCommand::Clear();

		if (HasPrimaryCamera())
		{
			Ref<Camera> cameraData = GetComponent<CameraComponent>(m_PrimaryCameraEntityID).CameraData;
			TransformComponent cameraTransform = GetComponent<TransformComponent>(m_PrimaryCameraEntityID);

			Rosewood::Renderer::BeginScene(*cameraData, cameraTransform);

			Rosewood::Renderer::PrepareShader(m_Shader);
			auto group = m_Registry.group<TransformComponent, MeshComponent>();
			for (auto&& [entityID, transform, mesh] : group.each())
			{
				Rosewood::Renderer::Submit(m_Shader, mesh.VAO, transform);
			}

			Rosewood::Renderer::EndScene();
		}

	}

	void Scene::SetPrimaryCameraID(EntityID newPrimaryCameraEntityID)
	{
		if (m_Registry.valid(newPrimaryCameraEntityID) && HasComponent<CameraComponent>(newPrimaryCameraEntityID) && HasComponent<TransformComponent>(newPrimaryCameraEntityID))
		{
			m_PrimaryCameraEntityID = newPrimaryCameraEntityID;
		}
	}

	bool Scene::HasPrimaryCamera() const
	{
		return (m_Registry.valid(m_PrimaryCameraEntityID) && HasComponent<CameraComponent>(m_PrimaryCameraEntityID) && HasComponent<TransformComponent>(m_PrimaryCameraEntityID));
	}

	EntityID Scene::GetPrimaryCameraID() const
	{
		return m_PrimaryCameraEntityID;
	}

	EntityID Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::DestroyEntity(EntityID entityID)
	{	
		m_Registry.destroy(entityID);
	}
}