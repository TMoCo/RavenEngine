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
		//GetModule<Raven::ImGuiEngine>()->Prepare();

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
	GetModule<Raven::RenderModule>()->Update(dt);

}

void Engine::OnRender()
{
	Raven::RenderModule* renderer = GetModule<Raven::RenderModule>();

	// Prepare Rendering...
	renderer->BeginRender();

	// Render...
	renderer->Render();

	// Render Cleanup...
	renderer->EndRender();
}

void Engine::LoadModules()
{
  // Create...
	CreateModule<Raven::RenderModule>();
	CreateModule<Raven::ImGuiEngine>();
	CreateModule<Raven::Window>("Raven");
	CreateModule<Raven::SceneManager>();


  // Initialize - Here order matter.
	InitializeModule<Raven::Window>();
	InitializeModule<Raven::RenderModule>();
	InitializeModule<Raven::ImGuiEngine>();
	InitializeModule<Raven::SceneManager>();

}



void Engine::DestoryModules()
{
  // Destroy - Here order matter.
  DestroyModule<Raven::RenderModule>();
}

std::future<bool> Engine::Post(const std::function<bool()>& callback)
{
	std::promise<bool> promise;
	std::future<bool> future = promise.get_future();
	std::lock_guard<std::mutex> locker(executeMutex);
	executeQueue.emplace(std::move(promise), callback);
	return future;
}
