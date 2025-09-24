#pragma once

#include "Entity.h"
#include "Rosewood/Scene/Components.h"
#include "entt/entt.hpp"

namespace Rosewood
{

	class Registry
	{
		friend class Scene;

	public:
		Registry() = default;
		~Registry() = default;

		EntityID CreateEntity();
		void DestroyEntity(EntityID entityID);

		template<typename T>
		bool HasComponent(EntityID entityID) const;
		template<typename T>
		void AddTag(EntityID entityID);
		template<typename T, typename... Args>
		T& AddComponent(EntityID entityID, Args&&... args);
		template<typename T>
		T& GetComponent(EntityID entityID);
		template<typename T>
		void RemoveComponent(EntityID entityID);

		template<typename... Components>
		auto GetAllEntitiesWith();

	private:
		entt::registry m_Registry;
	};

	template<typename T>
	inline bool Registry::HasComponent(EntityID entityID) const
	{
		return m_Registry.any_of<T>(entityID);
	}

	template<typename T>
	inline void Registry::AddTag(EntityID entityID)
	{
		m_Registry.emplace<T>(entityID);
	}

	template<typename T, typename... Args>
	inline T& Registry::AddComponent(EntityID entityID, Args&&... args)
	{
		return m_Registry.emplace<T>(entityID, std::forward<Args>(args)...);
	}

	template<typename T>
	inline T& Registry::GetComponent(EntityID entityID)
	{
		return m_Registry.get<T>(entityID);
	}
	template<typename T>
	inline void Registry::RemoveComponent(EntityID entityID)
	{
		m_Registry.remove<T>(entityID);
	}

	template<typename... Components>
	inline auto Registry::GetAllEntitiesWith()
	{
		return m_Registry.view<Components...>();
	}
}