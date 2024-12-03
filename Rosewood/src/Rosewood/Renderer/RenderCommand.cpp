#include "rwpch.h"
#include "RenderCommand.h"

#include "Rosewood/Renderer/OpenGLRendererAPI.h"

namespace Rosewood {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}