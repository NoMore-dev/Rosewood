#include "rwpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Rosewood
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RW_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RW_CORE_ASSERT(status, "Failed to initialized Glad!");

		RW_CORE_INFO("OpenGL Info:");
		RW_CORE_INFO("	Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		RW_CORE_INFO("	Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		RW_CORE_INFO("	Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}