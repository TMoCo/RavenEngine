//

#include "Engine.h"
#include "IModule.h"

#include "Window/Window.h"
#include "Render/RenderModule.h"
#include "ImGui/ImGuiEngine.h"
#include "Scene/SceneManager.h"
#include <GLFW/glfw3.h>

#include <memory>



Engine::Engine()
{

}



Engine::~Engine()
{

}


Engine& Engine::Get()
{
	static std::unique_ptr<Engine> instance(CreateEngine());
	return *instance;
}


void Engine::Initialize()
{
	
	
  // Module
  LoadModules();

}



int Engine::Run()
{
	  static double enginetime = 0;

	  auto win = GetModule<Raven::Window>();

	  // Main Loop...
	  while (!win->ShouldClose())
	  {
		//need to be refactored
		win->PollEvent();
		GetModule<Raven::ImGuiEngine>()->Prepare();

		// Draw..
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//time could be refactor with chrono
		double dt = glfwGetTime() - enginetime;
		enginetime = glfwGetTime();

		GetModule<Raven::SceneManager>()->Apply();

		OnUpdate(dt);
		OnRender();//To be modified by Renderer.
		OnImGui();
		GetModule<Raven::ImGuiEngine>()->Render();
	
		win->SwapBuffers();
	  }
	  // Clean Up..
	  DestoryModules();
	  return 0;
}


void Engine::OnImGui()
{

}

void Engine::OnUpdate(float dt)
{
	GetModule<RenderModule>()->Update();

}

void Engine::OnRender()
{
}

void Engine::LoadModules()
{
  // Create...
	CreateModule<RenderModule>();
	CreateModule<Raven::ImGuiEngine>();
	CreateModule<Raven::Window>("Raven");
	CreateModule<Raven::SceneManager>();


  // Initialize - Here order matter.
	InitializeModule<Raven::Window>();
	InitializeModule<RenderModule>();
	InitializeModule<Raven::ImGuiEngine>();
	InitializeModule<Raven::SceneManager>();

}



void Engine::DestoryModules()
{
  // Destroy - Here order matter.
  DestroyModule<RenderModule>();
}

std::future<bool> Engine::Post(const std::function<bool()>& callback)
{
	std::promise<bool> promise;
	std::future<bool> future = promise.get_future();
	std::lock_guard<std::mutex> locker(executeMutex);
	executeQueue.emplace(std::move(promise), callback);
	return future;
}
