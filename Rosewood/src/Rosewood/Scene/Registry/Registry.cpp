#include "rwpch.h"
#include "Registry.h"

namespace Rosewood
{
	EntityID Registry::CreateEntity()
	{
		return m_Registry.create();
	}

	void Registry::DestroyEntity(EntityID entityID)
	{
		m_Registry.destroy(entityID);
	}
}