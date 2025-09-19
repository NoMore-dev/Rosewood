#pragma once

#include "RendererAPI.h"

namespace Rosewood {

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void SetCullMode(CullMode cullMode)
		{
			s_RendererAPI->SetCullMode(cullMode);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawInstances(const Ref<VertexArray>& vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawInstances(vertexArray, count);
		}

		inline static void DrawBatch(const Ref<Batch> batch)
		{
			s_RendererAPI->DrawBatch(batch);
		}

		inline static void SetViewportSize(int width, int height)
		{
			s_RendererAPI->SetViewportSize(width, height);
		}

		inline static void Finish()
		{
			s_RendererAPI->Finish();
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}
