#pragma once

#include <glm/glm.hpp>

#include "Rosewood/Renderer/VertexArray/VertexArray.h"
#include "Rosewood/Renderer/Batch/Batch.h"
#include "Rosewood/Renderer/RendererEnums.h"


namespace Rosewood 
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		virtual void Init() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetCullMode(CullMode cullMode) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawInstances(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;

		virtual void DrawBatch(const Ref<Batch>& batch) = 0;

		virtual void SetViewportSize(int width, int height) = 0;

		virtual void Finish() = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}