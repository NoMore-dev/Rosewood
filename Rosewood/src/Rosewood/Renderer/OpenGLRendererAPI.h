#pragma once

#include "Rosewood/Renderer/RendererAPI.h"

namespace Rosewood {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void SetCullMode(CullMode cullMode) override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		void DrawInstances(const Ref<VertexArray>& vertexArray, uint32_t count) override;

		void DrawBatch(const Ref<Batch>& batch) override;

		void SetViewportSize(int width, int height) override;

		void Finish() override;
	};


}