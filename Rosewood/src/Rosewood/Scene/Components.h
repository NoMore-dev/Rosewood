#pragma once
#include "Rosewood/Transform/Transform.h"
#include "Rosewood/Renderer/Mesh/Mesh.h"
#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Camera/Camera.h"
#include "Rosewood/Renderer/Lighting/Light.h"

namespace Rosewood
{

	struct TransformComponent
	{
		TransformData& Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(TransformData& transform)
			: Transform(transform) {}

		operator const glm::mat4& () { return Transform.GetMatrix(); }
	};



	//#### Rendering #####################################################################

	struct CameraComponent
	{
		Camera& CameraData;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(Camera& camera)
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