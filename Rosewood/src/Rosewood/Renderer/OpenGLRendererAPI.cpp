#include "rwpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Rosewood {

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawInstances(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
	}

	void OpenGLRendererAPI::DrawBatch(const Ref<Batch>& batch)
	{
		//struct DrawElementsIndirectCommand
		//{
		//	uint32_t  count;
		//	uint32_t  instanceCount;
		//	uint32_t  firstIndex;
		//	uint32_t  baseVertex;
		//	uint32_t  baseInstance; 
		//};

		//DrawElementsIndirectCommand* commands = new DrawElementsIndirectCommand[batch->GetObjectCount()];
		//uint32_t firstIndex = 0;
		//uint32_t baseVertex = 0;
		//for (uint32_t i = 0; i < batch->GetObjectCount(); i++)
		//{
		//	commands[i] = DrawElementsIndirectCommand{ batch->GetIndexCount(i), 1, firstIndex, baseVertex, 0 };
		//	firstIndex += batch->GetIndexCount(i);
		//	baseVertex += batch->GetVertexCount(i);
		//}

		batch->GetVertexArray()->Bind();
		batch->GetTransformBuffer()->Bind();

		void* commands = (void*)batch->GetCommands();
		glMultiDrawElementsIndirect(
			GL_TRIANGLES,
			GL_UNSIGNED_INT,
			commands,
			batch->GetObjectCount(),
			0);
	}

	void OpenGLRendererAPI::SetViewportSize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}