#pragma once

#include <Rosewood.h>

class CameraController
{
public:
	CameraController(Rosewood::Ref<Rosewood::Scene> scene);
	~CameraController() = default;

	void OnStart();
	void OnUpdate(float dt);
	void OnEvent(Rosewood::Event& event);

private:
	bool OnMouseScrolledEvent(Rosewood::MouseScrolledEvent& event);

	void UpdateCameraTransform();

private:
	Rosewood::Ref<Rosewood::Scene> m_Scene;
	glm::vec3 m_CameraRotation = glm::vec3(0.f);

	float m_RotationSpeed = 0.002f;
	float m_ZoomSpeed = 5.f;
	float m_MinFov = 20.f;
	float m_MaxFov = 130.f;
};