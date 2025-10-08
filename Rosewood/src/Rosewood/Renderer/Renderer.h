#pragma once

#include "Rosewood/Renderer/RenderCommand.h"
#include "Rosewood/Renderer/Camera/Camera.h"
#include "Rosewood/Renderer/Shader/Shader.h"
#include "Rosewood/Renderer/Lighting/Light.h"
#include "Rosewood/Renderer/Batch/Batch.h"
#include "Rosewood/Renderer/Buffer/Buffer.h"
#include "Rosewood/Renderer/Framebuffer/Framebuffer.h"
#include "Rosewood/Renderer/Material/Material.h"
#include "Rosewood/Renderer/Lighting/ShadowMap/ShadowMap.h"

namespace Rosewood {

	class Renderer
	{

	public:
		static void Init();

		static void BeginScene(Camera& camera, glm::mat4 cameraTransform, std::vector<PointLight> pointlights, std::vector<SpotLight> spotLights, std::vector<DirectionalLight> dirLights);
		static void DrawScene();
		static void EndScene();

		static void Submit(const Ref<Material> material, const Ref<VertexArray> vertexArray, const glm::mat4* transform, bool castShadows);

		static void SetRenderFramebuffer(Ref<Framebuffer> framebuffer);
		static void ClearRenderFramebuffer();
		static Ref<Framebuffer> GetRenderFramebuffer() { return s_SceneData.RenderFramebuffer; }

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static void ComputeShadowMap();

	private:
		struct DrawData {
			Ref<VertexArray> vertexArray;
			Ref<Material> material;
			const glm::mat4* transform = nullptr;
			bool CastShadows = false;
		};

		struct SceneData {
			Ref<UniformBuffer> CameraUniformBuffer;
			Ref<UniformBuffer> LightsUniformBuffer;
			Ref<UniformBuffer> InstanceUniformBuffer;

			uint32_t DrawDataCount = 0;
			std::array<DrawData, 10000> DrawDataContainer;

			Ref<Framebuffer> RenderFramebuffer;

			Ref<ShadowMap> ShadowMap;
			Ref<Shader> ShadowMapShader;
			Ref<UniformBuffer> ShadowMapLightBuffer;

			glm::vec4 Direction;
		};

		static SceneData s_SceneData;

	public:
		static const int MAX_POINT_LIGHTS = 64;
		static const int MAX_SPOT_LIGHTS = 64;
		static const int MAX_DIR_LIGHTS = 8;
	};


}