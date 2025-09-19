#pragma once

#include "Rosewood/Renderer/Buffer/Buffer.h"

namespace Rosewood
{

	// VertexBuffer //////////////////////////////////////////////////////////////

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(BYTE* vertices, uint32_t size, BufferLayout& layout);
		~OpenGLVertexBuffer();

		void Bind() override;
		void Unbind() override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};


	// IndexBuffer //////////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() override;
		void Unbind() override;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	// StorageBuffer //////////////////////////////////////////////////////////////

	class OpenGLStorageBuffer : public StorageBuffer
	{
	public:
		OpenGLStorageBuffer(BYTE* data, uint32_t size);
		~OpenGLStorageBuffer();

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_BufferID;
	};

	// UniformBuffer //////////////////////////////////////////////////////////////

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size);
		~OpenGLUniformBuffer();

		void BindToBindingPoint(uint32_t bindingPoint) override;

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t m_BufferID;
	};

}