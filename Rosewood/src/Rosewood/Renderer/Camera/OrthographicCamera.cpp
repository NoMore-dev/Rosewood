#include "rwpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Rosewood {

	OrthographicCamera::OrthographicCamera(float size, float ratio, float zNear, float zFar)
		: m_Settings({ size, ratio, zNear, zFar })
	{
		RecomputeProjectionMatrix();
	}

	void OrthographicCamera::RecomputeProjectionMatrix()
	{
		glm::vec4 params = { -1.f*m_Settings.ratio, 1.f*m_Settings.ratio, -1.f, 1.f };
		params *= m_Settings.size;
		m_ProjectionMatrix = glm::ortho(params.x, params.y, params.z, params.w, m_Settings.zNear, m_Settings.zFar);
	}
	 
}
