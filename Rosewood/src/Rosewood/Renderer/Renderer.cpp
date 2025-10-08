#include "rwpch.h"
#include "Renderer.h"
#include "Rosewood/Utils/Conversions.h"

namespace Rosewood {

	Renderer::SceneData Renderer::s_SceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
		
		s_SceneData.RenderFramebuffer = nullptr;

		s_SceneData.DrawDataCount = 0;

		s_SceneData.CameraUniformBuffer = UniformBuffer::Create(64 * 2);
		s_SceneData.CameraUniformBuffer->BindToBindingPoint(0);

		s_SceneData.LightsUniformBuffer = UniformBuffer::Create(16 + MAX_POINT_LIGHTS * sizeof(PointLight) 
																	+ MAX_DIR_LIGHTS   * sizeof(DirectionalLight)
																	+ MAX_SPOT_LIGHTS  * sizeof(SpotLight));
		s_SceneData.LightsUniformBuffer->BindToBindingPoint(1);

		s_SceneData.InstanceUniformBuffer = UniformBuffer::Create(64);
		s_SceneData.InstanceUniformBuffer->BindToBindingPoint(3);


		ShadowMapSpecification shadowMapSpec;
			shadowMapSpec.Width = 4172;
			shadowMapSpec.Height = 4172;
		s_SceneData.ShadowMap = ShadowMap::Create(shadowMapSpec);
		
		ShaderSpecification shaderSpec;
			shaderSpec.Name = "ShadowMap Shader";
			shaderSpec.VertexPath = "../../shaders/ShadowMap/ShadowMap.vert";
			shaderSpec.FragmentPath = "../../shaders/ShadowMap/ShadowMap.frag";
		s_SceneData.ShadowMapShader = Shader::Create(shaderSpec);

		s_SceneData.ShadowMapLightBuffer = UniformBuffer::Create(Utils::SizeToUint32(sizeof(glm::mat4)));
		s_SceneData.ShadowMapLightBuffer->BindToBindingPoint(4);
	}

	void Renderer::BeginScene(Camera& camera, glm::mat4 cameraTransform, std::vector<PointLight> pointlights, std::vector<SpotLight> spotLights, std::vector<DirectionalLight> dirLights)
	{
		glm::mat4x4 CameraViewMatrix = glm::inverse(cameraTransform);
		glm::mat4x4 CameraViewProjectionMatrix = camera.GetProjectionMatrix() * CameraViewMatrix;

		std::vector<BYTE> cameraData = std::vector<BYTE>(64*2);
		memcpy(cameraData.data(), &CameraViewMatrix, 64);
		memcpy(cameraData.data()+64, &CameraViewProjectionMatrix, 64);

		s_SceneData.CameraUniformBuffer->SetData(cameraData.data(), 64 * 2, 0);

		// Transforming lights vectors data from world space to view space
		for (auto& light : pointlights)
		{
			light.Position.w = 1.f;
			light.Position = CameraViewMatrix * light.Position;
		}
		for (auto& light : spotLights)
		{
			light.Position.w = 1.f;
			light.Position = CameraViewMatrix * light.Position;
			light.Direction.w = 0.f;
			light.Direction = CameraViewMatrix * light.Direction;
		}
		for (auto& light : dirLights)
		{
			light.Direction.w = 0.f;
			light.Direction = CameraViewMatrix * light.Direction;
			s_SceneData.Direction = light.Direction;
		}

		// Send lighting data to uniform buffer 
		// TODO : Assert number of lights not greater than maximum
		s_SceneData.LightsUniformBuffer->SetData(pointlights.data(), Utils::SizeToUint32(pointlights.size()) * sizeof(PointLight), 16);
		s_SceneData.LightsUniformBuffer->SetData(spotLights.data(), Utils::SizeToUint32(spotLights.size()) * sizeof(SpotLight), 16 + sizeof(PointLight)*MAX_POINT_LIGHTS);
		s_SceneData.LightsUniformBuffer->SetData(dirLights.data(), Utils::SizeToUint32(dirLights.size()) * sizeof(DirectionalLight), 16 + sizeof(PointLight) * MAX_POINT_LIGHTS + sizeof(SpotLight) * MAX_SPOT_LIGHTS);
			
		glm::vec4 lightsCount = glm::vec4(pointlights.size(), spotLights.size(), dirLights.size(), 0);
		s_SceneData.LightsUniformBuffer->SetData(&lightsCount, 16, 0);

		
	}

	void Renderer::DrawScene()
	{
		ComputeShadowMap();

		if (s_SceneData.RenderFramebuffer)
			s_SceneData.RenderFramebuffer->Bind();

		Rosewood::RenderCommand::SetClearColor({ 0.3f, 0.1f, 0.11f, 1.0f });
		Rosewood::RenderCommand::Clear();


		struct Comp {
			bool operator() (DrawData a, DrawData b) {
				uint32_t ar = a.material->GetID();
				uint32_t br = b.material->GetID();

				if (a.material->GetSpecification().Opacity != b.material->GetSpecification().Opacity)
				{
					return a.material->GetSpecification().Opacity < b.material->GetSpecification().Opacity;
				}

				return ar < br;
			}
		};

		if (s_SceneData.DrawDataCount == 0) return;

		std::sort(s_SceneData.DrawDataContainer.begin(), s_SceneData.DrawDataContainer.begin() + s_SceneData.DrawDataCount, Comp());

		uint32_t currentMaterial = 0;
		uint32_t i = 0;
		for (DrawData drawData : s_SceneData.DrawDataContainer)
		{
			if (i == s_SceneData.DrawDataCount)
				break;

			uint32_t newMaterial = drawData.material->GetID();
			if (newMaterial != currentMaterial || i==0)
			{
				drawData.material->GetShader()->Bind();
				drawData.material->UploadMaterialData();
				RenderCommand::SetCullMode(drawData.material->GetSpecification().CullMode);

				currentMaterial = newMaterial;
			}

			s_SceneData.InstanceUniformBuffer->SetData(drawData.transform, 64, 0);

			drawData.vertexArray->Bind();
			RenderCommand::DrawIndexed(drawData.vertexArray);

			i++;
		}
	}

	void Renderer::EndScene()
	{
		s_SceneData.DrawDataCount = 0;
	}

	void Renderer::Submit(const Ref<Material> material, const Ref<VertexArray> vertexArray, const glm::mat4* transform, bool castShadows)
	{
		s_SceneData.DrawDataContainer[s_SceneData.DrawDataCount] = DrawData{ vertexArray, material, transform, castShadows };
		s_SceneData.DrawDataCount++;
	}


	void Renderer::SetRenderFramebuffer(Ref<Framebuffer> framebuffer)
	{
		s_SceneData.RenderFramebuffer = framebuffer;
	}

	void Renderer::ClearRenderFramebuffer()
	{
		s_SceneData.RenderFramebuffer = nullptr;
	}
	 

	void Renderer::ComputeShadowMap()
	{
		float near_plane = 1.0f, far_plane = 50.f, size = 25.f;
		glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(glm::vec3(10.f, 10.f, 0.f), glm::vec3(s_SceneData.Direction), glm::vec3(0.f,1.f,0.f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		s_SceneData.ShadowMapLightBuffer->SetData(&lightSpaceMatrix, 64, 0);

		s_SceneData.ShadowMapShader->Bind();

		s_SceneData.ShadowMap->Bind();
		RenderCommand::Clear();

		uint32_t i = 0;
		for (DrawData drawData : s_SceneData.DrawDataContainer)
		{
			if (i == s_SceneData.DrawDataCount || !drawData.CastShadows)
				break;

			s_SceneData.InstanceUniformBuffer->SetData(drawData.transform, 64, 0);

			drawData.vertexArray->Bind();
			RenderCommand::DrawIndexed(drawData.vertexArray);

			i++;
		}

		s_SceneData.ShadowMap->Unbind();

		s_SceneData.ShadowMap->BindTexture(10);
	}

}