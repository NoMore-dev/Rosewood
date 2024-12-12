#include "rwpch.h"
#include "Renderer.h"

namespace Rosewood {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();

		s_SceneData->DrawDataCount = 0;

		s_SceneData->CameraUniformBuffer = UniformBuffer::Create(64, 0);
		s_SceneData->LightsUniformBuffer = UniformBuffer::Create(16 
																+ MAX_POINT_LIGHTS * sizeof(PointLight) 
																+ MAX_DIR_LIGHTS   * sizeof(DirectionalLight)
																+ MAX_SPOT_LIGHTS  * sizeof(SpotLight)
																, 1);
	}

	void Renderer::BeginScene(Camera& camera, glm::mat4 cameraTransform, std::vector<PointLight> pointlights, std::vector<SpotLight> spotLights, std::vector<DirectionalLight> dirLights)
	{
		glm::mat4x4 ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);
		s_SceneData->CameraUniformBuffer->SetData(&ViewProjectionMatrix, 64, 0);

		//Send lighting data to uniform buffer TODO : Assert number of lights not greater than maximum
		s_SceneData->LightsUniformBuffer->SetData(pointlights.data(), pointlights.size() * sizeof(PointLight), 16);
		s_SceneData->LightsUniformBuffer->SetData(spotLights.data(), spotLights.size() * sizeof(SpotLight), 16 + sizeof(PointLight)*MAX_POINT_LIGHTS);
		s_SceneData->LightsUniformBuffer->SetData(dirLights.data(), dirLights.size() * sizeof(DirectionalLight), 16 + sizeof(PointLight) * MAX_POINT_LIGHTS + sizeof(SpotLight) * MAX_SPOT_LIGHTS);
			
		glm::vec4 lightsCount = glm::vec4(pointlights.size(), spotLights.size(), dirLights.size(), 0);
		s_SceneData->LightsUniformBuffer->SetData(&lightsCount, 16, 0);
	}

	void Renderer::DrawScene()
	{
		struct Comp {
			bool operator() (DrawData a, DrawData b) {
				uint32_t ar = a.material->GetID();
				uint32_t br = b.material->GetID();

				return ar < br;
			}
		};

		if (s_SceneData->DrawDataCount == 0) return;

		std::sort(s_SceneData->DrawDataContainer.begin(), s_SceneData->DrawDataContainer.begin() + s_SceneData->DrawDataCount, Comp());

		uint32_t currentMaterial = s_SceneData->DrawDataContainer[0].material->GetID();
		s_SceneData->DrawDataContainer[0].material->GetShader()->Bind();
		s_SceneData->DrawDataContainer[0].material->GetShader()->UploadUniformFloat4("u_Color", s_SceneData->DrawDataContainer[0].material->GetColor());

		uint32_t i = 0;
		for (DrawData drawData : s_SceneData->DrawDataContainer)
		{
			if (i == s_SceneData->DrawDataCount)
				break;

			uint32_t newMaterial = drawData.material->GetID();
			if (newMaterial != currentMaterial)
			{
				drawData.material->GetShader()->Bind();
				drawData.material->GetShader()->UploadUniformFloat4("u_Color", s_SceneData->DrawDataContainer[0].material->GetColor());
				currentMaterial = newMaterial;
			}
			drawData.material->GetShader()->UploadUniformMat4("u_Transform", *drawData.transform);

			drawData.vertexArray->Bind();
			RenderCommand::DrawIndexed(drawData.vertexArray);

			i++;
		}
	}

	void Renderer::EndScene()
	{
		s_SceneData->DrawDataCount = 0;
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4* transform)
	{
		s_SceneData->DrawDataContainer[s_SceneData->DrawDataCount] = DrawData{ vertexArray, material, transform};
		s_SceneData->DrawDataCount++;
	}

	//void Renderer::SubmitInstances(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, int count)
	//{
	//	const glm::mat4& transform = glm::mat4(1.0f);

	//	shader->UploadUniformMat4("u_Transform", transform);

	//	vertexArray->Bind();
	//	RenderCommand::DrawInstances(vertexArray, count);
	//}

	//void Renderer::SubmitBatch(const Ref<Shader>& shader, const Ref<Batch>& batch)
	//{
	//	RenderCommand::DrawBatch(batch);
	//}

	//void Renderer::UploadLights(const Ref<Shader>& shader)
	//{
	//	shader->UploadUniformInt("u_PointLightsCount", s_SceneData->PointLightsCount);
	//	for (int i = 0; i < s_SceneData->PointLightsCount; i++)
	//	{
	//		std::string arrayString = "u_Pointlights[" + std::to_string(i) + "]";
	//		shader->UploadUniformFloat3(arrayString + ".position", s_SceneData->PointLights[i]->position);
	//		shader->UploadUniformFloat3(arrayString + ".ambient", s_SceneData->PointLights[i]->ambient);
	//		shader->UploadUniformFloat3(arrayString + ".diffuse", s_SceneData->PointLights[i]->diffuse);
	//		shader->UploadUniformFloat3(arrayString + ".specular", s_SceneData->PointLights[i]->specular);
	//		shader->UploadUniformFloat(arrayString + ".constant", s_SceneData->PointLights[i]->constant);
	//		shader->UploadUniformFloat(arrayString + ".linear", s_SceneData->PointLights[i]->linear);
	//		shader->UploadUniformFloat(arrayString + ".quadratic", s_SceneData->PointLights[i]->quadratic);
	//	}

	//	shader->UploadUniformInt("u_SpotLightsCount", s_SceneData->SpotLightsCount);
	//	for (int i = 0; i < s_SceneData->SpotLightsCount; i++)
	//	{
	//		std::string arrayString = "u_Spotlights[" + std::to_string(i) + "]";
	//		shader->UploadUniformFloat3(arrayString + ".position", s_SceneData->SpotLights[i]->position);
	//		shader->UploadUniformFloat3(arrayString + ".direction", s_SceneData->SpotLights[i]->direction);
	//		shader->UploadUniformFloat(arrayString + ".innerCutOff", s_SceneData->SpotLights[i]->innerCutOff);
	//		shader->UploadUniformFloat(arrayString + ".outerCutOff", s_SceneData->SpotLights[i]->outerCutOff);
	//		shader->UploadUniformFloat3(arrayString + ".ambient", s_SceneData->SpotLights[i]->ambient);
	//		shader->UploadUniformFloat3(arrayString + ".diffuse", s_SceneData->SpotLights[i]->diffuse);
	//		shader->UploadUniformFloat3(arrayString + ".specular", s_SceneData->SpotLights[i]->specular);
	//		shader->UploadUniformFloat(arrayString + ".constant", s_SceneData->SpotLights[i]->constant);
	//		shader->UploadUniformFloat(arrayString + ".linear", s_SceneData->SpotLights[i]->linear);
	//		shader->UploadUniformFloat(arrayString + ".quadratic", s_SceneData->SpotLights[i]->quadratic);
	//	}

	//	shader->UploadUniformInt("u_DirLightsCount", s_SceneData->DirectionalLightsCount);
	//	for (int i = 0; i < s_SceneData->DirectionalLightsCount; i++)
	//	{
	//		std::string arrayString = "u_Directionallights[" + std::to_string(i) + "]";
	//		shader->UploadUniformFloat3(arrayString + ".direction", s_SceneData->DirectionalLights[i]->direction);
	//		shader->UploadUniformFloat3(arrayString + ".ambient", s_SceneData->DirectionalLights[i]->ambient);
	//		shader->UploadUniformFloat3(arrayString + ".diffuse", s_SceneData->DirectionalLights[i]->diffuse);
	//		shader->UploadUniformFloat3(arrayString + ".specular", s_SceneData->DirectionalLights[i]->specular);
	//	}
	//}


}