#include "CameraController.h"

CameraController::CameraController(Rosewood::Ref<Rosewood::Scene> scene)
	: m_Scene(scene)
{
	Rosewood::TransformData cameraTransform = Rosewood::TransformData{};
	cameraTransform.SetTranslation(glm::vec3(50.f, 0.f, 0.f));
	cameraTransform.SetRotationEuler(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));
	Rosewood::Ref<Rosewood::PerspectiveCamera> camera = std::make_shared<Rosewood::PerspectiveCamera>();
	Rosewood::EntityID cameraEntityID = m_Scene->GetRegistry()->CreateEntity();
	m_Scene->GetRegistry()->AddComponent<Rosewood::TransformComponent>(cameraEntityID, cameraTransform);
	m_Scene->GetRegistry()->AddComponent<Rosewood::CameraComponent>(cameraEntityID, camera);
	m_Scene->SetPrimaryCameraID(cameraEntityID);
}


void CameraController::OnStart()
{
	m_CameraRotation.x = glm::pi<float>() / 12.f;
	UpdateCameraTransform();
}

void CameraController::OnUpdate(float dt)
{
	if (Rosewood::Input::IsMouseButtonPressed(RW_MOUSE_BUTTON_3))
	{
		Rosewood::Application::Get().GetWindow().DisableCursor();

		if (m_Scene->HasPrimaryCamera())
		{
			auto [deltaHori, deltaVer] = Rosewood::Input::GetMousePositionDelta();
			float deltaRotY = -deltaHori * m_RotationSpeed;
			float deltaRotX = deltaVer * m_RotationSpeed;

			m_CameraRotation.y += deltaRotY;
			m_CameraRotation.x += deltaRotX;
			UpdateCameraTransform();
		}
	}
	else Rosewood::Application::Get().GetWindow().RestoreCursor();
}

void CameraController::OnEvent(Rosewood::Event& event)
{
	Rosewood::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Rosewood::MouseScrolledEvent>(RW_BIND_EVENT_FN(CameraController::OnMouseScrolledEvent));
}

bool CameraController::OnMouseScrolledEvent(Rosewood::MouseScrolledEvent& event)
{
	Rosewood::EntityID cameraID = m_Scene->GetPrimaryCameraID();
	Rosewood::Ref<Rosewood::PerspectiveCamera> camera = m_Scene->GetRegistry()->GetComponent<Rosewood::CameraComponent>(cameraID).CameraRef;

	float newFov = camera->GetSettings().fov - m_ZoomSpeed*event.GetYOffset();
	newFov = std::min(std::max(newFov, m_MinFov), m_MaxFov);
	camera->SetFov(newFov);

	return true;
}

void CameraController::UpdateCameraTransform()
{
	glm::quat q1 = glm::quat(glm::vec3(0.f, m_CameraRotation.y, 0.f));
	glm::quat q2 = glm::quat(glm::vec3(0.f, 0.f, m_CameraRotation.x));
	glm::mat4 mat = glm::mat4_cast(q1) * glm::mat4_cast(q2) * glm::translate(glm::mat4(1.0f), glm::vec3(50.f, 0.f, 0.f)) * glm::mat4_cast(glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f)));

	Rosewood::EntityID cameraID = m_Scene->GetPrimaryCameraID();
	Rosewood::TransformComponent& cameraTransformComponent = m_Scene->GetRegistry()->GetComponent<Rosewood::TransformComponent>(cameraID);
	cameraTransformComponent.Transform.SetMatrix(mat);
}
