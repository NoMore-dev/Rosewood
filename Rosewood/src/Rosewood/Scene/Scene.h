#pragma once

#include "Registry/Registry.h"
#include "Rosewood/Scene/Systems/ComputeShaderSystem/ComputeShadersSystem.h"

namespace Rosewood
{
	class Scene
	{
	public:
		Scene::Scene();
		Scene::~Scene() = default;

		void Scene::OnUpdate(float dt);
		void Scene::Render();

		void SetPrimaryCameraID(EntityID newPrimaryCameraEntityID);
		bool HasPrimaryCamera() const;
		EntityID GetPrimaryCameraID() const;

		Ref<Registry> GetRegistry() { return m_Registry; }

	private:
		Ref<Registry> m_Registry;
		Ref<ComputeShadersSystem> m_ComputeShadersSystem;
		EntityID m_PrimaryCameraEntityID = Entity::null;
	};
}