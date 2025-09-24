#pragma once

#include "entt/entt.hpp"

namespace Rosewood
{
	using EntityID = entt::entity;
	namespace Entity
	{
		inline constexpr EntityID null = entt::null;
	}
}