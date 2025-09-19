#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace Rosewood
{

	class TransformData
	{
	public:
		TransformData() = default;
		TransformData(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
		TransformData(glm::mat4 transformMatrix);
		~TransformData() = default;

		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::quat& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetRotationEuler() const { return m_RotationEuler; }
		const glm::vec3& GetScale() const { return m_Scale; }

		glm::vec3 GetForward() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetLeft() const;

		const glm::mat4& GetMatrix() { if (m_DirtyFlag) RecalculateMatrix(); return m_TransformMatrix; }
		void SetMatrix(const glm::mat4& matrix) { m_TransformMatrix = matrix; DecomposeMatrix(); }

		void SetTranslation(const glm::vec3& translation) { m_Translation = translation; m_DirtyFlag = true; }
		void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; m_RotationEuler = glm::eulerAngles(m_Rotation); m_DirtyFlag = true; }
		void SetRotationEuler(const glm::vec3& rotation) { m_RotationEuler = rotation; m_Rotation = glm::quat(m_RotationEuler); m_DirtyFlag = true; }
		void SetScale(const glm::vec3& scale) { m_Scale = scale; m_DirtyFlag = true; }

		void RotateAround(float angle, glm::vec3 axis);

	private:
		void RecalculateMatrix();
		void DecomposeMatrix();

	private:
		glm::vec3 m_Translation = {};
		glm::quat m_Rotation = {};
		glm::vec3 m_RotationEuler = {};
		glm::vec3 m_Scale = { 1.f, 1.f, 1.f };

		glm::mat4 m_TransformMatrix = {1.f};
		bool m_DirtyFlag = false;
	};

}