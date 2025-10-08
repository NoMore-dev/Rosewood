#include "SceneHierachy.h"


SceneHierachy::SceneHierachy(Rosewood::Ref<Rosewood::Scene> scene)
	: m_Scene(scene) 
{}


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