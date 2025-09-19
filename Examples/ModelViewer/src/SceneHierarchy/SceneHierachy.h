#pragma once

#include <Rosewood.h>

class SceneHierachy
{
public:
	SceneHierachy(Rosewood::Ref<Rosewood::Scene> scene);
	~SceneHierachy() = default;

	const std::vector<Rosewood::EntityID>& GetEntitiesInHierarchy();
	void AddEntityToHierachy(Rosewood::EntityID entityID);
	void RemoveEntityFromHierachy(uint32_t index);

	Rosewood::EntityID GetSelectedEntity();
	void DeselectSelectedEntity();
	void SetSelectedEntity(Rosewood::EntityID selectedID);

	Rosewood::Ref<Rosewood::Scene> GetScene() { return m_Scene; }

private:
	Rosewood::Ref<Rosewood::Scene> m_Scene;
	std::vector<Rosewood::EntityID> m_EntitiesInHierarchy;
	Rosewood::EntityID m_SelectedEntityID = Rosewood::Entity::null;
};
