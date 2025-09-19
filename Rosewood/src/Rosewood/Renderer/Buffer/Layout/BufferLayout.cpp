#include "rwpch.h"
#include "BufferLayout.h"

#include "Rosewood/Renderer/Shader/OpenGLShaderDataType.h"

namespace Rosewood
{

	void BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
		}
		m_Stride = offset;
	}

	void BufferLayout::CalculateOffsetAndStrideWithAlignement()
	{
		uint32_t offset = 0;
		for (auto& element : m_Elements)
		{
			uint32_t alignement = OpenGLShaderDataTypeAlignement(element.Type);
			offset += (alignement - (offset % alignement)) % alignement;
			element.Offset = offset;
			offset += element.Size;
		}
		m_Stride = offset;
	}

}