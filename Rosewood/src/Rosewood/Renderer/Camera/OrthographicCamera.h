#pragma once

#include"Camera.h"

namespace Rosewood {

	class OrthographicCamera : public Camera {

	public:
		OrthographicCamera(float size = 1.f, float ratio = 16.f / 9.f, float zNear = 0.f, float zFar = 1.f);

		const glm::mat4& GetProjectionMatrix() override { return m_ProjectionMatrix; }

	private:
		void RecomputeProjectionMatrix();

	private:
		glm::mat4 m_ProjectionMatrix = {};

		struct OrthographicSettings
		{
			float size;
			float ratio;
			float zNear;
			float zFar;
		};
		OrthographicSettings m_Settings;

	};

}