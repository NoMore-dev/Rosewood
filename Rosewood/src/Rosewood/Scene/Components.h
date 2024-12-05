#pragma once
#include "Rosewood/Scene/Transform/Transform.h"
#include "Rosewood/Renderer/Mesh/Mesh.h"
#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Camera/Camera.h"
#include "Rosewood/Renderer/Lighting/Light.h"

namespace Rosewood
{
	//#### Common ########################################################################

	struct TagComponent
	{

	};

	struct TransformComponent
	{
		TransformData Transform = TransformData{};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(TransformData transform)
			: Transform(transform) {}
		TransformComponent(glm::mat4 transformMatrix)
			: Transform(transformMatrix) {}

		operator const glm::mat4& () { return Transform.GetMatrix(); }
	};

	//#### Rendering #####################################################################

	struct CameraComponent
	{
		Ref<Camera> CameraData;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(Ref<Camera> camera)
			: CameraData(camera) {}
	};

	struct MeshComponent
	{
		Ref<VertexArray> VAO;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(Ref<VertexArray> va)
			: VAO(va) {}
	};

	struct PointLightComponent
	{
		PointLight& lightData;
	};
	struct SpottLightComponent
	{
		SpotLight& lightData;
	};
	struct DirectionalLightComponent
	{
		DirectionalLight& lightData;
	};

}