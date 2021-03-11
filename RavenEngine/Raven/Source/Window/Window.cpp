//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Window/Window.h"
#include <GL/glew.h>
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_DOUBLEBUFFER, 8);
		glfwWindowHint(GLFW_SAMPLES, 0);

	//	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

		glfwWindow = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(glfwWindow);
		glewInit();//opengl loader init;
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