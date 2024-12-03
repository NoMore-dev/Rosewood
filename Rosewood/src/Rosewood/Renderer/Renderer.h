#pragma once

#include "Rosewood/Renderer/RenderCommand.h"
#include "Rosewood/Renderer/Camera/Camera.h"
#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Lighting/Light.h"
#include "Rosewood/Renderer/Batch/Batch.h"

namespace Rosewood {

	class Renderer
	{

	public:
		static void Init();

		static void BeginScene(Camera& camera, glm::mat4 cameraTransform);
		static void EndScene();

		static void PrepareShader(const Ref<Shader>& shader);
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void SubmitInstances(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, int count);
		static void SubmitBatch(const Ref<Shader>& shader, const Ref<Batch>& batch);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static void UploadLights(const Ref<Shader>& shader);

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;

			std::vector<PointLight*> PointLights;
			int PointLightsCount = 0;
			std::vector<SpotLight*> SpotLights;
			int SpotLightsCount = 0;
			std::vector<DirectionalLight*> DirectionalLights;
			int DirectionalLightsCount = 0;
		};

		static SceneData* m_SceneData;

		static const int MAX_POINT_LIGHTS = 5;
		static const int MAX_SPOT_LIGHTS = 5;
		static const int MAX_DIR_LIGHTS = 1;
	};


}