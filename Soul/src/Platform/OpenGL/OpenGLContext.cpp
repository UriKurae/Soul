#include "slpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Soul
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		SL_CORE_ASSERT(windowHandle, "Handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SL_CORE_ASSERT(status, "Failed to initialize Glad");

		SL_CORE_INFO("OpenGL Info:");
		SL_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		SL_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		SL_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}