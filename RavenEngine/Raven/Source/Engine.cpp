//

#include "Engine.h"
#include "IModule.h"

#include "Window/Window.h"
#include "Render/RenderModule.h"
#include "ResourceManager/ResourceManager.h"
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
	const std::string imagePath = "C:\\Users\\Tommy\\Pictures\\SML2_Wario_500.png"; // test path for image loading (500 x 500)

	GetModule<Raven::ResourceManager>()->LoadResource(imagePath, Raven::EResourceType::RT_Image);

	// check that the resource is in the registry
	
	if (GetModule<Raven::ResourceManager>()->HasResource(imagePath))
	{
		std::cout << "Resource is in registry\n";
		Raven::Texture2D* loaded = GetModule<Raven::ResourceManager>()->GetResource(imagePath);
		std::cout << "w: " << loaded->width << " h: " << loaded->height << '\n';
	}
	else
	{
		std::cout << "error somwhere...\n";
	}



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
	CreateModule<Raven::ResourceManager>();
	CreateModule<RenderModule>();
	CreateModule<Raven::ImGuiEngine>();
	CreateModule<Raven::Window>("Raven");
	CreateModule<Raven::SceneManager>();


  // Initialize - Here order matter.
	InitializeModule<Raven::ResourceManager>();
	InitializeModule<Raven::Window>();
	InitializeModule<RenderModule>();
	InitializeModule<Raven::ImGuiEngine>();
	InitializeModule<Raven::SceneManager>();
}



void Engine::DestoryModules()
{
  // Destroy - Here order matter.
  DestroyModule<RenderModule>();
  DestroyModule<Raven::ResourceManager>();
}

std::future<bool> Engine::Post(const std::function<bool()>& callback)
{
	std::promise<bool> promise;
	std::future<bool> future = promise.get_future();
	std::lock_guard<std::mutex> locker(executeMutex);
	executeQueue.emplace(std::move(promise), callback);
	return future;
}
