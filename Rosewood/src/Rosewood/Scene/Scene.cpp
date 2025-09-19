#include "rwpch.h"
#include "scene.h"
#include "Rosewood/Renderer/Renderer.h"
#include "Rosewood/Scene/Components.h"

namespace Rosewood
{
	Scene::Scene()
	{}

	void Scene::OnUpdate(float dt)
	{
	}

	void Scene::Render()
	{
		if (HasPrimaryCamera())
		{
			Ref<Camera> cameraData = GetComponent<CameraComponent>(m_PrimaryCameraEntityID).CameraRef;
			TransformComponent cameraTransform = GetComponent<TransformComponent>(m_PrimaryCameraEntityID);

			std::vector<PointLight> pointLights;
			std::vector<SpotLight> spotLights;
			std::vector<DirectionalLight> dirLights;
			auto view = m_Registry.view<PointLightComponent>();
			for (auto&& [entityID, light] : view.each())
			{
				pointLights.push_back(light.LightData);
				TransformData transform = GetComponent<TransformComponent>(entityID).Transform;
				pointLights.back().Position = glm::vec4(transform.GetTranslation(), 1.f);
			}

			Rosewood::Renderer::BeginScene(*cameraData, cameraTransform, pointLights, spotLights, dirLights);


			auto group = m_Registry.group<TransformComponent, RenderableObject3D>();
			for (auto&& [entityID, transform, renderableObject3D] : group.each())
			{
				for (int i = 0; i < renderableObject3D.Surfaces.size(); i++)
					Rosewood::Renderer::Submit(renderableObject3D.Materials[i], renderableObject3D.Surfaces[i], &transform.Transform.GetMatrix());
			}

			Rosewood::Renderer::DrawScene();

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