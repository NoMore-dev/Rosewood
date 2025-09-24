#pragma once

#include "Rosewood/Scene/Systems/System.h"

namespace Rosewood
{

	class ComputeShadersSystem : public System
	{
	public:
		ComputeShadersSystem(Ref<Registry> registry);
		~ComputeShadersSystem() = default;

		void OnUpdate() override;

	private:
		Ref<Registry> m_Registry = nullptr;
	};

}