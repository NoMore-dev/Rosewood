#pragma once

#include "Rosewood/Transform/Transform.h"

#include "glm/mat4x4.hpp"

namespace Rosewood
{

	class Camera 
	{
	public:
		virtual ~Camera() {};

		virtual const glm::mat4& GetProjectionMatrix() = 0;
		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetViewProjectionMatrix() = 0;

		virtual TransformData& GetTransform() = 0;
	};

}