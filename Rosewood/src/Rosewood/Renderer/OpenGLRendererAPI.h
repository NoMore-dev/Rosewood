#pragma once

#include "Rosewood/Renderer/RendererAPI.h"

namespace Rosewood {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawInstances(const Ref<VertexArray>& vertexArray, uint32_t count) override;

		virtual void DrawBatch(const Ref<Batch>& batch) override;

		virtual void SetViewportSize(int width, int height) override;
	};


}