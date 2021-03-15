//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Window/Window.h"
#include "Event/Event.h"
#include "Event/WindowEvent.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine.h"

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
		InitEventsCallback();
	}


	void Window::Destroy()
	{
		glfwDestroyWindow(glfwWindow);
	}

	void Window::InitEventsCallback()
	{
		glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* win, int32_t w, int32_t h) {
			Engine::Get().GetEventDispatcher().PostEvent(std::make_unique<WindowResizeEvent>(w, h));
			});

		glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int32_t btnId, int32_t state, int32_t mods) {
			double x;
			double y;
			glfwGetCursorPos(window, &x, &y);

			auto btn = -1;
			switch (btnId)
			{
			case GLFW_MOUSE_BUTTON_LEFT:btn = 0; break;
			case GLFW_MOUSE_BUTTON_MIDDLE:btn = 1; break;
			case GLFW_MOUSE_BUTTON_RIGHT:btn = 2; break;
			}

			if (state == GLFW_PRESS || state == GLFW_REPEAT)
			{
				Engine::Get().GetEventDispatcher().PostEvent(std::make_unique<MouseClickEvent>(btn, x, y));
			}
			if (state == GLFW_RELEASE)
			{
				Engine::Get().GetEventDispatcher().PostEvent(std::make_unique<MouseReleaseEvent>(btn, x, y));
			}
			});


		glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow*, double x, double y) {
			Engine::Get().GetEventDispatcher().PostEvent(std::make_unique<MouseMoveEvent>(x, y));
		});

		glfwSetScrollCallback(glfwWindow, [](GLFWwindow* win, double xOffset, double yOffset) {
			double x;
			double y;
			glfwGetCursorPos(win, &x, &y);
			Engine::Get().GetEventDispatcher().PostEvent(std::make_unique<MouseScrolledEvent>(xOffset, yOffset,
				x, y));
			});

		glfwSetCharCallback(glfwWindow, [](GLFWwindow* window, unsigned int keycode) {
			Engine::Get().GetEventDispatcher().PostEvent(std::make_unique<CharInputEvent>(KeyCode::Id(keycode), (char)keycode));
			});

		glfwSetKeyCallback(glfwWindow, [](GLFWwindow*, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
			switch (action)
			{
			case GLFW_PRESS:
			{
				Engine::Get().GetEventDispatcher().PostEvent(std::make_unique <KeyPressedEvent>(static_cast<KeyCode::Id>(key), 0));
				break;
			}
			case GLFW_RELEASE:
			{
				Engine::Get().GetEventDispatcher().PostEvent(std::make_unique <KeyReleasedEvent>(static_cast<KeyCode::Id>(key)));
				break;
			}
			case GLFW_REPEAT:
			{
				Engine::Get().GetEventDispatcher().PostEvent(std::make_unique <KeyPressedEvent>(static_cast<KeyCode::Id>(key), 1));
				break;
			}
			}
		});

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