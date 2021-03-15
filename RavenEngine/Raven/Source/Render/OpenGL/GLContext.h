#pragma once




#include "Render/RenderSurface.h"


// Forward Declaration...
struct GLFWwindow;




namespace Raven
{

	// OpenGL Context.
	class GLContext
	{
	public:
		// Construct.
		GLContext();

		// Set the glfw context. 
		void SetGLFWContext(GLFWwindow* context);

		// Return the required surface by the render
		static RenderSurface GetSurface();

		// Make this context current.
		void MakeCurrent();

	private:
		// The glfw window used to create the context.
		GLFWwindow* glfw_context;
	};


}
