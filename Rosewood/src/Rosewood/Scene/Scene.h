#pragma once

#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Camera/PerspectiveCamera.h"

#include "entt/entt.hpp"

#include <utility>


namespace Rosewood
{
	using EntityID = entt::entity;

	class Scene
	{
	public:
		Scene::Scene() = default;
		Scene::~Scene() = default;

		void Scene::OnUpdate(float dt);
		void Scene::Render();

		void SetPrimaryCameraID(EntityID newPrimaryCameraEntityID);
		bool HasPrimaryCamera() const;
		EntityID GetPrimaryCameraID() const;

		EntityID CreateEntity();
		void DestroyEntity(EntityID entityID);
		template<typename T>
		bool HasComponent(EntityID entityID) const;
		template<typename T, typename... Args>
		T& AddComponent(EntityID entityID, Args&&... args);
		template<typename T>
		T& GetComponent(EntityID entityID);
		template<typename T>
		void RemoveComponent(EntityID entityID);

	public:
		Ref<Shader> m_Shader;

	private:
		entt::registry m_Registry;
		EntityID m_PrimaryCameraEntityID = entt::null;
	};

	template<typename T>
	inline bool Scene::HasComponent(EntityID entityID) const
	{
		return m_Registry.any_of<T>(entityID);
	}

	template<typename T, typename... Args>
	inline T& Scene::AddComponent(EntityID entityID, Args&&... args)
	{
		return m_Registry.emplace<T>(entityID, std::forward<Args>(args)...);
	}

	template<typename T>
	inline T& Scene::GetComponent(EntityID entityID)
	{
		return m_Registry.get<T>(entityID);
	}
	template<typename T>
	inline void Scene::RemoveComponent(EntityID entityID)
	{
		m_Registry.remove<T>(entityID);
	}
}