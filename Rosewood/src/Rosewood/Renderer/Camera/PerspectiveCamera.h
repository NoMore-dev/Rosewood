#pragma once

#include"Camera.h"

namespace Rosewood {

	class PerspectiveCamera : public Camera {

	public:
		PerspectiveCamera(float fov = 50.f, float ratio = 16.f/9.f, float zNear = 0.1f, float zFar = 10000.f);
		~PerspectiveCamera() = default;

		const glm::mat4& GetProjectionMatrix() override { return m_ProjectionMatrix; }
		glm::mat4 GetViewMatrix() override { return glm::inverse(m_Transform.GetMatrix()); }
		glm::mat4 GetViewProjectionMatrix() override { return GetProjectionMatrix()*GetViewMatrix(); }

		TransformData& GetTransform() override  { return m_Transform; }

		void SetFov(float fov) { m_Settings.fov = fov; RecomputeProjectionMatrix();};

	private:
		void RecomputeProjectionMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		TransformData m_Transform = TransformData();

		struct PerspectiveSettings
		{
			float fov;
			float ratio;
			float zNear;
			float zFar;
		};
		PerspectiveSettings m_Settings;
	};

}