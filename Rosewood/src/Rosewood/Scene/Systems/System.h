#pragma once

#include "Rosewood/Scene/Registry/Registry.h"

namespace Rosewood
{

	class System
	{
	public:
		virtual void OnUpdate() = 0;
	};

}