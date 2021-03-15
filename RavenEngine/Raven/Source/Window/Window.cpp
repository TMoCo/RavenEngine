//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Window/Window.h"
#include "Render/RenderModule.h"

#include <GLFW/glfw3.h>



namespace Raven
{

	Window::Window(const std::string& title)
		:title(title)
	{

	}

	void Window::Initialize()
	{
		glfwInit();

		// Render Surface Properties.
		RenderSurface surface = RenderModule::GetRequiredRenderSurface();

		//
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, surface.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, surface.minorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, surface.coreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_RED_BITS, surface.redBits);
		glfwWindowHint(GLFW_GREEN_BITS, surface.greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, surface.blueBits);
		glfwWindowHint(GLFW_ALPHA_BITS, surface.alphaBits);
		glfwWindowHint(GLFW_SAMPLES, surface.samples);
		glfwWindowHint(GLFW_DOUBLEBUFFER, surface.doubleBuffer ? GLFW_TRUE : GLFW_FALSE);

		//	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

		// Create Window...
		glfwWindow = glfwCreateWindow(1280, 820, title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(glfwWindow);

	}


	void Window::Destroy()
	{
		glfwDestroyWindow(glfwWindow);
	}
	std::pair<float, float> Window::GetWindowScale()
	{
		float x, y;
		glfwGetWindowContentScale(glfwWindow, &x, &y);
		return {x,y};
	}
	void Window::PollEvent()
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(glfwWindow);
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(glfwWindow);
	}

};