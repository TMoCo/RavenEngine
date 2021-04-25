//
#include <memory>


#include <GLFW/glfw3.h>


#include "Engine.h"
#include "IModule.h"

#include "Utilities/StringUtils.h"

#include "Window/Window.h"

#include "Render/RenderModule.h"
#include "Render/RenderTarget.h"

#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/Terrain.h"

#include "ImGui/ImGuiEngine.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/System/SystemManager.h"
#include "Scene/System/GUISystem.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Entity/Entity.h"

#include "GUI/GUIModule.h"

#include "ProceduralGenerator/TerrainGeneration.h"

#include "Devices/Input.h"
#include "Scripts/LuaSystem.h"
#include "Animation/AnimationSystem.h"
#include "Scripts/LuaVirtualMachine.h"


namespace Raven 
{
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
			Input::GetInput()->ResetPressed();

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
		eventDispatcher.DispatchEvents();
		systemManager->OnUpdate(dt, GetModule<Raven::SceneManager>()->GetCurrentScene());
		GetModule<Raven::SceneManager>()->GetCurrentScene()->OnUpdate(dt);

		// Update Render...
		GetModule<RenderModule>()->Update(dt);
	}

	void Engine::OnRender()
	{
		Raven::RenderModule* renderer = GetModule<Raven::RenderModule>();
	
		// The Scene To Render.
		Scene* scene = GetModule<Raven::SceneManager>()->GetCurrentScene();

		// TODO: Better Sync between render, window and targets.
		glm::ivec2 extent = GetModule<Window>()->GetFramebufferSize();
	
		// Prepare Rendering...
		renderer->BeginRender(scene, extent);
	
		// Render...
		renderer->Render();
	
		// Render Cleanup...
		renderer->EndRender();
	}

	void Engine::LoadModules()
	{
		// Create...
		systemManager = std::make_unique<SystemManager>();

		CreateModule<Raven::ResourceManager>();
		CreateModule<Raven::LuaVirtualMachine>();
		CreateModule<Raven::RenderModule>();
		CreateModule<Raven::ImGuiEngine>();
		CreateModule<Raven::Window>("Raven");
		CreateModule<Raven::SceneManager>();
		CreateModule<Raven::GUIModule>();
		CreateModule<Raven::TerrainGeneration>();

		Input::Create();

		// Initialize - Here order matter.
		InitializeModule<Raven::Window>();
		InitializeModule<Raven::LuaVirtualMachine>();
		InitializeModule<Raven::ResourceManager>();
		InitializeModule<Raven::RenderModule>();
		InitializeModule<Raven::ImGuiEngine>();
		InitializeModule<Raven::SceneManager>();
		InitializeModule<Raven::GUIModule>();
		InitializeModule<Raven::TerrainGeneration>();

//############	Register your system here ######################

		auto guiSystem = GetSystemManager()->AddSystem<GUISystem>();
		GetSystemManager()->AddSystem<LuaSystem>();
		GetSystemManager()->AddSystem<AnimationSystem>()->OnInit();
		guiSystem->OnInit();
	
//############	Register your system here ######################

	}

	void Engine::DestoryModules()
	{
		// Destroy - Here order matter.
		DestroyModule<TerrainGeneration>();
		DestroyModule<RenderModule>();
		DestroyModule<SceneManager>();
		DestroyModule<GUIModule>();
		DestroyModule<ImGuiEngine>();
		DestroyModule<ResourceManager>();
		DestroyModule<Window>();
		DestroyModule<LuaVirtualMachine>();
	}

	std::future<bool> Engine::Post(const std::function<bool()>& callback)
	{
		std::promise<bool> promise;
		std::future<bool> future = promise.get_future();
		std::lock_guard<std::mutex> locker(executeMutex);
		executeQueue.emplace(std::move(promise), callback);
		return future;
	}

	void Engine::OnSceneCreated(Scene* scene)
	{

	}

	void Engine::NewGameScene()
	{
		// get the terrain generator and generate a height map
		auto generator = GetModule<TerrainGeneration>();
		generator->GenerateSquareGradient(100, 100);
		generator->GenerateNoise(100, 100, FileFormat::PNG);
		std::string path("heightmap.png");
		// load the generated height map into resource manager
		GetModule<ResourceManager>()->LoadResource<Texture2D>(path);

		Terrain* terrain = new Terrain(GetModule<ResourceManager>()->GetResource<Texture2D>(path).get());

		std::cout << terrain->heightMap->width << '\n';
		std::cout << terrain->heightMap->height << " \n";

		Scene* newScene = new Scene("TerrainScene");
		newScene->dynamic = true;

		auto entity = newScene->CreateEntity("terrain");
		entity.AddComponent<TerrainComponent>(path, std::shared_ptr<Terrain>(terrain));

		uint32_t sceneIdx = GetModule<SceneManager>()->AddScene(newScene);
		GetModule<SceneManager>()->SwitchScene(sceneIdx);

	}
};
