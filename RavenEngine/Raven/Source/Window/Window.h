//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <memory>
#include "IModule.h"



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
		inline auto GetNativeWindow() { return glfwWindow; }

		void PollEvent();
		bool ShouldClose();
		void SwapBuffers();
	private:
		GLFWwindow* glfwWindow = nullptr;
		std::string title;
	};
};