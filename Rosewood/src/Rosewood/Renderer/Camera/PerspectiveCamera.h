#pragma once

#include"Camera.h"

namespace Rosewood {

	class PerspectiveCamera : public Camera {
	public:
		struct Settings
		{
			float fov;
			float ratio;
			float zNear;
			float zFar;
		};

	public:
		PerspectiveCamera(float fov = 50.f, float ratio = 16.f/9.f, float zNear = 0.1f, float zFar = 10000.f);
		~PerspectiveCamera() = default;

		const glm::mat4& GetProjectionMatrix() override { return m_ProjectionMatrix; }

		void SetFov(float fov) { m_Settings.fov = fov; RecomputeProjectionMatrix();};
		const Settings& GetSettings() const { return m_Settings; }

	private:
		void RecomputeProjectionMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;

		Settings m_Settings;
	};

}