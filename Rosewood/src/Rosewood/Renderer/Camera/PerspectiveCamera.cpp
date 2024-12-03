#include "rwpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Rosewood {

	PerspectiveCamera::PerspectiveCamera(float fov, float ratio, float zNear, float zFar)
		: m_ProjectionMatrix(glm::perspective(glm::radians(fov), ratio, zNear, zFar)), m_Settings({fov, ratio, zNear, zFar})
	{
	}

	void PerspectiveCamera::RecomputeProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Settings.fov), m_Settings.ratio, m_Settings.zNear, m_Settings.zFar);
	}

}
