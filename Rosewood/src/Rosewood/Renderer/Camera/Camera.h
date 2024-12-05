#pragma once

#include "Rosewood/Scene/Transform/Transform.h"

#include "glm/mat4x4.hpp"

namespace Rosewood
{

	class Camera 
	{
	public:
		virtual ~Camera() {};

		virtual const glm::mat4& GetProjectionMatrix() = 0;
	};

}