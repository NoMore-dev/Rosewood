#pragma once
#include "Rosewood/Scene/Transform/Transform.h"
#include "Rosewood/Renderer/Mesh/Mesh.h"
#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Camera/Camera.h"
#include "Rosewood/Renderer/Lighting/Light.h"
#include "Rosewood/Renderer/Material/Material.h"

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
		Ref<Camera> CameraRef;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(Ref<Camera> camera)
			: CameraRef(camera) {}
	};

	struct MaterialComponent
	{
		Ref<Material> MaterialRef;

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent(Ref<Material> material)
			: MaterialRef(material) {}
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
		PointLight LightData;

		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(PointLight& lightData)
			: LightData(lightData) {}
		PointLightComponent(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation)
		{
			LightData = PointLight(
				glm::vec4(0.f, 0.f, 0.f, 0.f),
				glm::vec4(ambient, 0.f),
				glm::vec4(diffuse, 0.f),
				glm::vec4(specular, 0.f),
				glm::vec4(attenuation, 0.f));
		}
	};
	struct SpotLightComponent
	{
		SpotLight LightData;

		SpotLightComponent() = default;
		SpotLightComponent(const SpotLightComponent&) = default;
		SpotLightComponent(SpotLight& lightData)
			: LightData(lightData) {}
		SpotLightComponent(float nearCutOff, float farCutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation)
		{
			LightData = SpotLight(
				glm::vec4(0.f, 0.f, 0.f, 0.f),
				glm::vec4(0.f, -1.f, 0.f, 0.f),
				glm::vec4(nearCutOff, farCutOff, 0.f, 0.f),
				glm::vec4(ambient, 0.f),
				glm::vec4(diffuse, 0.f),
				glm::vec4(specular, 0.f),
				glm::vec4(attenuation, 0.f));
		}
	};
	struct DirectionalLightComponent
	{
		DirectionalLight LightData;

		DirectionalLightComponent() = default;
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent(DirectionalLight& lightData)
			: LightData(lightData) {}
		DirectionalLightComponent(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		{
			LightData = DirectionalLight(
				glm::vec4(0.f, 0.f, 0.f, 0.f),
				glm::vec4(ambient, 0.f),
				glm::vec4(diffuse, 0.f),
				glm::vec4(specular, 0.f));
		}
	};

}