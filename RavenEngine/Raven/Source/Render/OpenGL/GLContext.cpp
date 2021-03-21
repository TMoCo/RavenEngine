#include "GLContext.h"



#include "GL/glew.h"
#include "GLFW/glfw3.h"



namespace Raven {







GLContext::GLContext()
	: glfw_context(nullptr)
{

}


void GLContext::SetGLFWContext(GLFWwindow* context)
{
	glfw_context = context;
}


RenderSurface GLContext::GetSurface()
{
	RenderSurface surface;

	// OpenGL 3.3 core profile.
	surface.majorVersion = 3;
	surface.minorVersion = 3;
	surface.coreProfile = true;

	// RGBA - 32Bit
	surface.redBits = 8;
	surface.greenBits = 8;
	surface.blueBits = 8;
	surface.alphaBits = 8;

	// Double Buffer.
	surface.doubleBuffer = true;

	// No MSAA, we are using framebuffers.
	surface.samples = 0;

	return surface;
}


void GLContext::MakeCurrent()
{
	glfwMakeContextCurrent(glfw_context);
}



} // End of namespace Raven.
