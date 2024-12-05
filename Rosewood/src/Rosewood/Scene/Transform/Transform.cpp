#include "rwpch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Rosewood
{

	TransformData::TransformData(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale):
		m_Translation(translation), m_Rotation(rotation), m_Scale(scale)
	{
		RecalculateMatrix();
	}

	TransformData::TransformData(glm::mat4 transformMatrix):
		m_TransformMatrix(transformMatrix)
	{
	}

	glm::vec3 TransformData::GetForward() const
	{
		return m_Rotation * glm::vec3(0.f, 0.f, 1.f);
	}

	glm::vec3 TransformData::GetUp() const
	{
		return m_Rotation * glm::vec3(0.f, 1.f, 0.f);
	}

	glm::vec3 TransformData::GetLeft() const
	{
		return m_Rotation * glm::vec3(1.f, 0.f, 0.f);
	}

	void TransformData::RotateAround(float angle, glm::vec3 axis)
	{
		m_RotationEuler += angle*axis;
		m_Rotation = glm::quat(m_RotationEuler);
		m_DirtyFlag = true;
	}

	void TransformData::RecalculateMatrix()
	{
		m_TransformMatrix =
				glm::translate(glm::mat4(1.0f), m_Translation) *

				glm::mat4_cast(m_Rotation) *

				glm::scale(glm::mat4(1.0f), m_Scale) ;

		m_DirtyFlag = false;
	}

	void TransformData::DecomposeMatrix()
	{

	}

}