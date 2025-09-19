#pragma once

#include "Rosewood/Renderer/Buffer/Layout/BufferLayout.h"

namespace Rosewood 
{
	// VertexBuffer //////////////////////////////////////////////////////////////

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(BYTE* vertices, uint32_t size, BufferLayout& layout);
	};


	// IndexBuffer //////////////////////////////////////////////////////////////

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

		virtual uint32_t GetCount() const = 0;
	};

	// StorageBuffer //////////////////////////////////////////////////////////////

	class StorageBuffer
	{
	public:
		virtual ~StorageBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<StorageBuffer> Create(BYTE* data, uint32_t size);
	};

	// UniformBuffer //////////////////////////////////////////////////////////////

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual void BindToBindingPoint(uint32_t bindingPoint) = 0;

		static Ref<UniformBuffer> Create(uint32_t size);
	};

}