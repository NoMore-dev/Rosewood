#pragma once

#include "Rosewood/Renderer/Shader/ShaderDataType.h"

namespace Rosewood
{
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
		{}
	};


	enum LayoutStandard
	{
		PACKED,
		STD140
	};

	class BufferLayout
	{
	public:
		BufferLayout() {};

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		BufferLayout(const std::vector<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		BufferLayout(LayoutStandard layoutStandard, const std::initializer_list<BufferElement>& elements)
			: m_LayoutStandard(layoutStandard), m_Elements(elements)
		{
			switch (layoutStandard)
			{
			case Rosewood::PACKED:
				CalculateOffsetAndStride(); return;
			case Rosewood::STD140:
				CalculateOffsetAndStrideWithAlignement(); return;
			}
			
			RW_CORE_ASSERT(false, "Buffer layout standard not supported!")
		}

		BufferLayout(LayoutStandard layoutStandard, const std::vector<BufferElement>& elements)
			: m_LayoutStandard(layoutStandard), m_Elements(elements)
		{
			switch (layoutStandard)
			{
			case Rosewood::PACKED:
				CalculateOffsetAndStride(); return;
			case Rosewood::STD140:
				CalculateOffsetAndStrideWithAlignement(); return;
			}

			RW_CORE_ASSERT(false, "Buffer layout standard not supported!")
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
		LayoutStandard m_LayoutStandard = LayoutStandard::PACKED;

		void CalculateOffsetAndStride();

		void CalculateOffsetAndStrideWithAlignement();
	};
}