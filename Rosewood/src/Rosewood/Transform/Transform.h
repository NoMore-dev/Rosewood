#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Rosewood
{

	class TransformData
	{
	public:
		TransformData(glm::vec3 translation = {}, glm::vec3 rotation = {}, glm::vec3 scale = { 1.f, 1.f, 1.f });
		~TransformData() = default;

		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::vec3& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetScale() const { return m_Scale; }

		glm::vec3 GetForward() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetLeft() const;

		const glm::mat4& GetMatrix() { if (m_DirtyFlag) RecalculateMatrix(); return m_TransformMatrix; }

		void SetTranslation(const glm::vec3& translation) { m_Translation = translation; m_DirtyFlag = true; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; m_DirtyFlag = true; }
		void SetScale(const glm::vec3& scale) { m_Scale = scale; m_DirtyFlag = true; }

	private:
		void RecalculateMatrix();

	private:
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 m_TransformMatrix = {};
		bool m_DirtyFlag = false;
	};

}