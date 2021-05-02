//
#include <memory>


#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp>

#include "Engine.h"
#include "IModule.h"

#include "Utilities/StringUtils.h"

#include "Window/Window.h"

#include "Render/RenderDebug.h"
#include "Render/RenderModule.h"
#include "Render/RenderTarget.h"

#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/Terrain.h"

#include "ImGui/ImGuiEngine.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Entity/Entity.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Component/RigidBody.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/System/SystemManager.h"
#include "Scene/System/PhysicsSystem.h"
#include "Scene/System/GUISystem.h"

#include "Physics/PhysicsModule.h"

#include "GUI/GUIModule.h"

#include "ProceduralGenerator/TerrainGeneration.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/Resources/MaterialShader.h"
#include "ResourceManager/Resources/Material.h"


#include "Devices/Input.h"
#include "Scripts/LuaSystem.h"
#include "Scripts/LuaComponent.h"
#include "Animation/AnimationSystem.h"
#include "Scripts/LuaVirtualMachine.h"

#include "Core/Camera.h"




namespace Raven 
{
	Ptr<Material> TESTING_MAT;




	Engine::Engine()
		: engineTime(0.0f)
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
			double dt = glfwGetTime() - engineTime;
			engineTime = glfwGetTime();

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

		// Update Render...
		GetModule<RenderModule>()->Update(dt);

		// ~TESTING---------------------------------------------------------------------------------------------
		if (TESTING_MAT)
		{
			static float roughnessTime = 0.0f;
			roughnessTime += dt;
		}

		auto lightsEttView = GetModule<Raven::SceneManager>()->GetCurrentScene()->GetRegistry().group<Light>(entt::get<Transform>);

		//
		bool isSearchForSun = true;

		// Iterate over all lights in the scene.
		for (auto entity : lightsEttView)
		{
			const auto& [light, trans] = lightsEttView.get<Light, Transform>(entity);

			if (light.type == 0)
				continue;

			GetModule<RenderModule>()->GetDebug()->DrawBox(light.position, glm::vec3(10.0), light.color);
		}

		// ~TESTING---------------------------------------------------------------------------------------------




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
		// delete previous physics world
		auto physics = GetModule<PhysicsModule>();
		physics->DestroyWorld();

		// now create a new one for the new scene
		physics->CreateWorld();
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
		auto& luaScript = entity1.AddComponent<LuaComponent>();
		luaScript.SetFilePath("./scripts/PlayerController.lua");
		luaScript.LoadScript();

		// a transform
		auto& transform1 = entity1.GetOrAddComponent<Transform>();

		// and a rigidbody component
		auto& rb1 = entity1.AddComponent<RigidBody>();
		rb1.EnableGravity(false);
		rb1.SetMass(1.0);
		// configure its shapes
		//rb1.AddCollider(new BoxCollider());

		LOGV(physics->GetCurrentWorld()->getNbRigidBodies());

		{
			auto& lightEntity = newScene->CreateEntity("THE_SUN");
			auto& lightComp = lightEntity.GetOrAddComponent<Light>();
			auto& tr = lightEntity.GetOrAddComponent<Transform>();

			lightComp.type = (int32_t)LightType::DirectionalLight;
			lightComp.color = glm::vec4(1.0f, 0.95f, 0.8f, 1.0f);
			lightComp.intensity = 0.0f;
			lightComp.direction = glm::normalize(glm::vec3(-1.0f));
		}

		{
			Ptr<Mesh> plane = Ptr<Mesh>(MeshFactory::CreateQuad());

			auto meshEntity = newScene->CreateEntity("Plane");
			auto& tr = meshEntity.GetOrAddComponent<Transform>();
			tr.SetLocalPosition(glm::vec3(0.0f, -4.3f, 0.0f));
			tr.SetLocalScale(glm::vec3(3000.0f));
			tr.SetLocalOrientation(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f));

			auto& model = meshEntity.GetOrAddComponent<Model>();
			model.AddMesh(plane);
		}

		{
			auto camEntity = newScene->CreateEntity("Camera");
			auto& tr = camEntity.GetOrAddComponent<Transform>();
			tr.SetLocalPosition(glm::vec3(0.0f, 10.f, 0.0f));
			tr.SetLocalScale(glm::vec3(1.0f));
			tr.SetLocalOrientation(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f));
			auto& camera = camEntity.AddComponent<Camera>();
			camera.SetFov(90.f);
			camera.SetFar(100);
			camera.SetNear(0.01);
			camera.SetAspectRatio(4 / 3.f);

			newScene->SetOverrideCamera(&camera);		
		}

		// Switch the scene....
		uint32_t sceneIdx = GetModule<SceneManager>()->AddScene(newScene);
		// load scene using its id
		GetModule<SceneManager>()->SwitchScene(sceneIdx);
	}
};
