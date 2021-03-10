#include "GLContext.h"



#include "GL/glew.h"




using namespace Raven;







GLContext::GLContext()
	: glfw_context(nullptr)
{

}



void GLContext::SetGLFWContext(GLFWwindow* context)
{
	glfw_context = context;
}


GLSurface GLContext::GetSurface()
{
	GLSurface surface;

	surface.majorVersion = 3;
	surface.minorVersion = 3;
	surface.coreProfile = true;

	surface.redBits = 8;
	surface.greenBits = 8;
	surface.blueBits = 8;
	surface.alphaBits = 8;

	surface.doubleBuffer = true;
	surface.samples = 0;

	return surface;
}
