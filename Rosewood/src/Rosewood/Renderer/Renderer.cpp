#include "rwpch.h"
#include "Renderer.h"

namespace Rosewood {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();

		m_SceneData->PointLights = std::vector<PointLight*>(MAX_POINT_LIGHTS);
		m_SceneData->SpotLights = std::vector<SpotLight*>(MAX_SPOT_LIGHTS);
		m_SceneData->DirectionalLights = std::vector<DirectionalLight*>(MAX_DIR_LIGHTS);
	}

	void Renderer::BeginScene(Camera& camera, glm::mat4 cameraTransform)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);

		m_SceneData->PointLights[0] = new PointLight{
										glm::vec3(10.f,10.f,0.f),
										glm::vec3(.1f),
										glm::vec3(.5f),
										glm::vec3(0.f),
										1.f, .0003f, 0.0001f
		};
		m_SceneData->PointLightsCount = 1;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::PrepareShader(const Ref<Shader>& shader)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		UploadLights(shader);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::SubmitInstances(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, int count)
	{
		const glm::mat4& transform = glm::mat4(1.0f);

		shader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawInstances(vertexArray, count);
	}

	void Renderer::SubmitBatch(const Ref<Shader>& shader, const Ref<Batch>& batch)
	{
		RenderCommand::DrawBatch(batch);
	}

	void Renderer::UploadLights(const Ref<Shader>& shader)
	{
		shader->UploadUniformInt("u_PointLightsCount", m_SceneData->PointLightsCount);
		for (int i = 0; i < m_SceneData->PointLightsCount; i++)
		{
			std::string arrayString = "u_Pointlights[" + std::to_string(i) + "]";
			shader->UploadUniformFloat3(arrayString + ".position", m_SceneData->PointLights[i]->position);
			shader->UploadUniformFloat3(arrayString + ".ambient", m_SceneData->PointLights[i]->ambient);
			shader->UploadUniformFloat3(arrayString + ".diffuse", m_SceneData->PointLights[i]->diffuse);
			shader->UploadUniformFloat3(arrayString + ".specular", m_SceneData->PointLights[i]->specular);
			shader->UploadUniformFloat(arrayString + ".constant", m_SceneData->PointLights[i]->constant);
			shader->UploadUniformFloat(arrayString + ".linear", m_SceneData->PointLights[i]->linear);
			shader->UploadUniformFloat(arrayString + ".quadratic", m_SceneData->PointLights[i]->quadratic);
		}

		shader->UploadUniformInt("u_SpotLightsCount", m_SceneData->SpotLightsCount);
		for (int i = 0; i < m_SceneData->SpotLightsCount; i++)
		{
			std::string arrayString = "u_Spotlights[" + std::to_string(i) + "]";
			shader->UploadUniformFloat3(arrayString + ".position", m_SceneData->SpotLights[i]->position);
			shader->UploadUniformFloat3(arrayString + ".direction", m_SceneData->SpotLights[i]->direction);
			shader->UploadUniformFloat(arrayString + ".innerCutOff", m_SceneData->SpotLights[i]->innerCutOff);
			shader->UploadUniformFloat(arrayString + ".outerCutOff", m_SceneData->SpotLights[i]->outerCutOff);
			shader->UploadUniformFloat3(arrayString + ".ambient", m_SceneData->SpotLights[i]->ambient);
			shader->UploadUniformFloat3(arrayString + ".diffuse", m_SceneData->SpotLights[i]->diffuse);
			shader->UploadUniformFloat3(arrayString + ".specular", m_SceneData->SpotLights[i]->specular);
			shader->UploadUniformFloat(arrayString + ".constant", m_SceneData->SpotLights[i]->constant);
			shader->UploadUniformFloat(arrayString + ".linear", m_SceneData->SpotLights[i]->linear);
			shader->UploadUniformFloat(arrayString + ".quadratic", m_SceneData->SpotLights[i]->quadratic);
		}

		shader->UploadUniformInt("u_DirLightsCount", m_SceneData->DirectionalLightsCount);
		for (int i = 0; i < m_SceneData->DirectionalLightsCount; i++)
		{
			std::string arrayString = "u_Directionallights[" + std::to_string(i) + "]";
			shader->UploadUniformFloat3(arrayString + ".direction", m_SceneData->DirectionalLights[i]->direction);
			shader->UploadUniformFloat3(arrayString + ".ambient", m_SceneData->DirectionalLights[i]->ambient);
			shader->UploadUniformFloat3(arrayString + ".diffuse", m_SceneData->DirectionalLights[i]->diffuse);
			shader->UploadUniformFloat3(arrayString + ".specular", m_SceneData->DirectionalLights[i]->specular);
		}
	}

}