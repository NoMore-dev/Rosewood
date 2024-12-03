#pragma once

#include "rwpch.h"
#include "Rosewood/Renderer/Buffer/Buffer.h"

namespace Rosewood 
{

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Rosewood::Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Rosewood::Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Rosewood::Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Rosewood::Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}
