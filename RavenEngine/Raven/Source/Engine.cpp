//
#include <memory>


#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp>

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
#include "Scene/System/PhysicsSystem.h"
#include "Scene/System/GUISystem.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Component/RigidBody.h"
#include "Scene/Component/Model.h"
#include "Scene/Entity/Entity.h"

#include "Physics/PhysicsModule.h"

#include "GUI/GUIModule.h"

#include "ProceduralGenerator/TerrainGeneration.h"

#include "Devices/Input.h"
#include "Scripts/LuaSystem.h"

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
		// Dispatch events
		eventDispatcher.DispatchEvents();

		// Update the system managers
		systemManager->OnUpdate(dt, GetModule<Raven::SceneManager>()->GetCurrentScene());

		GetModule<Raven::SceneManager>()->GetCurrentScene()->OnUpdate(dt);

		// Update physics world, generates new transforms for entities with rigid bodies
		GetModule<PhysicsModule>()->Step(dt);

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
		CreateModule<Raven::PhysicsModule>();

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
		InitializeModule<Raven::PhysicsModule>();

//############	Register your system here ######################

		auto guiSystem = GetSystemManager()->AddSystem<GUISystem>();
		GetSystemManager()->AddSystem<PhysicsSystem>(); // register the physics system
		GetSystemManager()->AddSystem<LuaSystem>();
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
		Scene* newScene = new Scene("Physics Test World");
		newScene->dynamic = true;

		// delete previous physics world
		auto physics = GetModule<PhysicsModule>();
		physics->DestroyWorld();

		// now create a new one for the new scene
		physics->CreateWorld();

		LOGV(physics->GetCurrentWorld()->getNbRigidBodies());

		// creating a rigid body box
		auto entity1 = newScene->CreateEntity("Rigid Box 0");
		// give it a unit cube model
		auto& model1 = entity1.AddComponent<Model>();
		model1.SetPrimitiveType(PrimitiveType::Cube);
		model1.GetMeshes().emplace_back(MeshFactory::CreateCube());

		// a transform
		auto& transform1 = entity1.GetOrAddComponent<Transform>();

		// and a rigidbody component
		auto& rb1 = entity1.AddComponent<RigidBody>(physics->GetCurrentWorld(), transform1, RigidBodyType::Static);
		// configure its shapes
		rb1.AddCollider(new BoxCollider());

		char buf[32];

		for (size_t i = 0; i < 5; i++)
		{
			// creating another rigid body box that falls on other box
			sprintf(buf, "RigidBox %i", i+1);
			auto entity2 = newScene->CreateEntity(buf);
			// give it a unit cube model
			auto& model2 = entity2.AddComponent<Model>();
			model2.SetPrimitiveType(PrimitiveType::Cube);
			model2.GetMeshes().emplace_back(MeshFactory::CreateCube());

			// a transform
			auto& transform2 = entity2.GetOrAddComponent<Transform>();

			transform2.SetLocalPosition(glm::vec3(1.0f, 5.0f + i*5.0f, 0.0f));
			transform2.SetWorldMatrix(glm::mat4(1.0f));
			LOGV(glm::to_string(transform2.GetWorldMatrix()));
			// and a rigidbody component
			auto& rb2 = entity2.AddComponent<RigidBody>(physics->GetCurrentWorld(), transform2, RigidBodyType::Dynamic);
			rb2.AddCollider(new BoxCollider());
		}

		LOGV(physics->GetCurrentWorld()->getNbRigidBodies());
		
		// add scene and get its id
		uint32_t sceneIdx = GetModule<SceneManager>()->AddScene(newScene);
		// load scene using its id
		GetModule<SceneManager>()->SwitchScene(sceneIdx);
	}
};
