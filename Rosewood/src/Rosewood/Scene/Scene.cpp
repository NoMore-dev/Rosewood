#include "rwpch.h"
#include "scene.h"

#include "Rosewood/Renderer/Renderer.h"
#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Camera/PerspectiveCamera.h"
#include "Rosewood/Renderer/Framebuffer/Framebuffer.h"
#include "Rosewood/Renderer/Texture/Texture.h"
#include "Rosewood/Scene/Components.h"

namespace Rosewood
{
	Scene::Scene()
	{
		m_Registry = std::make_shared<Registry>();
		m_ComputeShadersSystem = std::make_shared<ComputeShadersSystem>(m_Registry);
	}

	void Scene::OnUpdate(float dt)
	{
		m_ComputeShadersSystem->OnUpdate();
	}

	void Scene::Render()
	{
		if (HasPrimaryCamera())
		{
			Ref<Camera> cameraData = m_Registry->GetComponent<CameraComponent>(m_PrimaryCameraEntityID).CameraRef;
			TransformComponent cameraTransform = m_Registry->GetComponent<TransformComponent>(m_PrimaryCameraEntityID);

			std::vector<PointLight> pointLights;
			std::vector<SpotLight> spotLights;
			std::vector<DirectionalLight> dirLights;
			auto view = m_Registry->m_Registry.view<PointLightComponent>();
			for (auto&& [entityID, light] : view.each())
			{
				pointLights.push_back(light.LightData);
				TransformData transform = m_Registry->GetComponent<TransformComponent>(entityID).Transform;
				pointLights.back().Position = glm::vec4(transform.GetTranslation(), 1.f);
			}

			Rosewood::Renderer::BeginScene(*cameraData, cameraTransform, pointLights, spotLights, dirLights);


			auto group = m_Registry->m_Registry.group<TransformComponent, RenderableObject3D>();
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
		if (m_Registry->m_Registry.valid(newPrimaryCameraEntityID) && m_Registry->HasComponent<CameraComponent>(newPrimaryCameraEntityID) && m_Registry->HasComponent<TransformComponent>(newPrimaryCameraEntityID))
		{
			m_PrimaryCameraEntityID = newPrimaryCameraEntityID;
		}
	}

	bool Scene::HasPrimaryCamera() const
	{
		return (m_Registry->m_Registry.valid(m_PrimaryCameraEntityID) && m_Registry->HasComponent<CameraComponent>(m_PrimaryCameraEntityID) && m_Registry->HasComponent<TransformComponent>(m_PrimaryCameraEntityID));
	}


	EntityID Scene::GetPrimaryCameraID() const
	{
		return m_PrimaryCameraEntityID;
	}
}