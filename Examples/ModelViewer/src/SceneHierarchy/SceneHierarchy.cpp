#include "SceneHierachy.h"


SceneHierachy::SceneHierachy(Rosewood::Ref<Rosewood::Scene> scene)
	: m_Scene(scene) 
{
	//Lights
	Rosewood::EntityID pointLightEntity = m_Scene->GetRegistry()->CreateEntity();
	AddEntityToHierachy(pointLightEntity);

	m_Scene->GetRegistry()->AddComponent<Rosewood::TagComponent>(pointLightEntity, "PointLight");
	m_Scene->GetRegistry()->AddComponent<Rosewood::PointLightComponent>(pointLightEntity, glm::vec4(.1f), glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.f, .0003f, 0.0001f));
	Rosewood::TransformData pointLightTransform = Rosewood::TransformData(glm::vec3(30.f, 20.f, 0.f), glm::quat(), glm::vec3(1.f));
	m_Scene->GetRegistry()->AddComponent<Rosewood::TransformComponent>(pointLightEntity, pointLightTransform);
}


const std::vector<Rosewood::EntityID>& SceneHierachy::GetEntitiesInHierarchy()
{
	return m_EntitiesInHierarchy;
}

void SceneHierachy::AddEntityToHierachy(Rosewood::EntityID entityID)
{
	m_EntitiesInHierarchy.push_back(entityID);
}

void SceneHierachy::RemoveEntityFromHierachy(uint32_t index)
{
	m_EntitiesInHierarchy.erase(m_EntitiesInHierarchy.begin() + index);
}

Rosewood::EntityID SceneHierachy::GetSelectedEntity()
{
	return m_SelectedEntityID;
}

void SceneHierachy::DeselectSelectedEntity()
{
	m_SelectedEntityID = Rosewood::Entity::null;
}

void SceneHierachy::SetSelectedEntity(Rosewood::EntityID selectedID)
{
	m_SelectedEntityID = selectedID;
}