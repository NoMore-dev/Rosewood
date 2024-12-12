#pragma once

#include "Rosewood/Renderer/RenderCommand.h"
#include "Rosewood/Renderer/Camera/Camera.h"
#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Lighting/Light.h"
#include "Rosewood/Renderer/Batch/Batch.h"
#include "Rosewood/Renderer/Buffer/Buffer.h"
#include "Rosewood/Renderer/Material/Material.h"

namespace Rosewood {

	class Renderer
	{

	public:
		static void Init();

		static void BeginScene(Camera& camera, glm::mat4 cameraTransform, std::vector<PointLight> pointlights, std::vector<SpotLight> spotLights, std::vector<DirectionalLight> dirLights);
		static void DrawScene();
		static void EndScene();

		static void Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4* transform);
/*		static void SubmitInstances(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, int count);
		static void SubmitBatch(const Ref<Shader>& shader, const Ref<Batch>& batch)*/;

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct DrawData {
			Ref<VertexArray> vertexArray;
			Ref<Material> material;
			const glm::mat4* transform;
		};

		struct SceneData {
			Ref<UniformBuffer> CameraUniformBuffer;
			Ref<UniformBuffer> LightsUniformBuffer;

			uint32_t DrawDataCount = 0;
			std::array<DrawData, 10000> DrawDataContainer;
		};

		static SceneData* s_SceneData;

	public:
		static const int MAX_POINT_LIGHTS = 64;
		static const int MAX_SPOT_LIGHTS = 64;
		static const int MAX_DIR_LIGHTS = 8;
	};


}