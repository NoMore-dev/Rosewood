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

	//normalize ?
	glm::vec3 TransformData::GetForward() const
	{
		glm::mat3 rotationMatrix =
			glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1));

		glm::vec3 forward = rotationMatrix * glm::vec3(0.f, 0.f, 1.f);

		return forward;
	}

	glm::vec3 TransformData::GetUp() const
	{
		glm::mat3 rotationMatrix =
			glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1));

		glm::vec3 up = rotationMatrix * glm::vec3(0.f, 1.f, 0.f);

		return up;
	}

	glm::vec3 TransformData::GetLeft() const
	{
		glm::mat3 rotationMatrix =
			glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1));

		glm::vec3 left = rotationMatrix * glm::vec3(1.f, 0.f, 0.f);

		return left;
	}

	void TransformData::RecalculateMatrix()
	{
		m_TransformMatrix =
				glm::translate(glm::mat4(1.0f), m_Translation) *

				glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1)) *

				glm::scale(glm::mat4(1.0f), m_Scale) ;

		m_DirtyFlag = false;
	}

}