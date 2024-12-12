#pragma once

#include "Rosewood/Renderer/Shader/Shader.h"

namespace Rosewood 
{

	// BufferLayout //////////////////////////////////////////////////////////////

	struct BufferElement 
	{
		std::string Name;
		ShaderDataType Type;
		bool Normalized;

		uint32_t Offset;
		uint32_t Size;
		uint32_t ComponentCount;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), ComponentCount(ShaderDataTypeComponentCount(type)), Offset(0), Normalized(normalized)
		{

		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

		void CalculateOffsetAndStride() 
		{	
			uint32_t offset = 0;
			for (auto& element : m_Elements) 
			{
				element.Offset = offset;
				offset += element.Size;
			}
			m_Stride = offset;
		}
	};


	// VertexBuffer //////////////////////////////////////////////////////////////

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(BYTE* vertices, uint32_t size);
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

		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
	};

}