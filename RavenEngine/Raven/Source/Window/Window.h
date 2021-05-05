//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <memory>
#include "IModule.h"

#include "glm/vec2.hpp"



struct GLFWwindow;

namespace Raven
{
	class Window : public IModule
	{
	public:
		Window(const std::string& title);
		static EModuleType GetModuleType() { return MT_Window; }
		virtual void Initialize() override;
		virtual void Destroy() override;

		void InitEventsCallback();

		inline auto GetNativeWindow() { return glfwWindow; }

		glm::vec2  GetWindowScale();
		glm::vec2  GetWindowSize();
		glm::ivec2 GetFramebufferSize();

		void PollEvent();
		bool ShouldClose();
		void SwapBuffers();
		void Close();

	private:
		GLFWwindow* glfwWindow = nullptr;
		std::string title;
	};
};