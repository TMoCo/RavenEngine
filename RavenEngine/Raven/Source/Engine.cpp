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
#include "Animation/AnimationSystem.h"
#include "Scripts/LuaVirtualMachine.h"





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

		// Update physics world, generates new transforms for entities with rigid bodies
		GetModule<PhysicsModule>()->Step(dt);

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
		auto terrainEntity = newScene->CreateEntity("terrain");
		terrainEntity.AddComponent<TerrainComponent>(path, std::shared_ptr<Terrain>(terrain));

		{
			auto& lightEntity = newScene->CreateEntity("THE_SUN");
			auto& lightComp = lightEntity.GetOrAddComponent<Light>();
			auto& tr = lightEntity.GetOrAddComponent<Transform>();

			lightComp.type = (int32_t)LightType::DirectionalLight;
			lightComp.color = glm::vec4(1.0f, 0.95f, 0.8f, 1.0f);
			lightComp.intensity = 0.0f;
			lightComp.direction = glm::normalize(glm::vec3(-1.0f));
		}


		// Basic Material...
		Ptr<MaterialShader> basicMatShader_1( new MaterialShader() );
		basicMatShader_1->SetDomain(ERenderShaderDomain::Mesh);
		basicMatShader_1->SetType(ERenderShaderType::Opaque);
		basicMatShader_1->SetMaterialFunction("shaders/Materials/BasicMaterial.glsl");
		basicMatShader_1->GetBlockInput().BeginUniformBlock("MaterialParamtersBlock");
		basicMatShader_1->GetBlockInput().AddInput(EShaderInputType::Vec4, "color");
		basicMatShader_1->GetBlockInput().AddInput(EShaderInputType::Vec4, "emission");
		basicMatShader_1->GetBlockInput().AddInput(EShaderInputType::Float, "roughness");
		basicMatShader_1->GetBlockInput().AddInput(EShaderInputType::Float, "metallic");
		basicMatShader_1->GetBlockInput().AddInput(EShaderInputType::Float, "specular");
		basicMatShader_1->GetBlockInput().AddInput(EShaderInputType::Float, "alpha");
		basicMatShader_1->GetBlockInput().EndUniformBlock();
		basicMatShader_1->AddSampler("baseColorTexture");
		basicMatShader_1->AddSampler("AOTexture");
		basicMatShader_1->AddSampler("metallicTexture");
		basicMatShader_1->AddSampler("roughnessTexture");
		basicMatShader_1->LoadOnGpu();

		// Basic Material...
		Ptr<MaterialShader> basicMatShader_2(new MaterialShader());
		basicMatShader_2->SetDomain(ERenderShaderDomain::Mesh);
		basicMatShader_2->SetType(ERenderShaderType::Translucent);
		basicMatShader_2->SetMaterialFunction("shaders/Materials/BasicMaterial.glsl");
		basicMatShader_2->GetBlockInput().BeginUniformBlock("MaterialParamtersBlock");
		basicMatShader_2->GetBlockInput().AddInput(EShaderInputType::Vec4, "color");
		basicMatShader_2->GetBlockInput().AddInput(EShaderInputType::Vec4, "emission");
		basicMatShader_2->GetBlockInput().AddInput(EShaderInputType::Float, "roughness");
		basicMatShader_2->GetBlockInput().AddInput(EShaderInputType::Float, "metallic");
		basicMatShader_2->GetBlockInput().AddInput(EShaderInputType::Float, "specular");
		basicMatShader_2->GetBlockInput().AddInput(EShaderInputType::Float, "alpha");
		basicMatShader_2->GetBlockInput().EndUniformBlock();
		basicMatShader_2->AddSampler("baseColorTexture");
		basicMatShader_2->AddSampler("AOTexture");
		basicMatShader_2->AddSampler("metallicTexture");
		basicMatShader_2->AddSampler("roughnessTexture");
		basicMatShader_2->LoadOnGpu();


		//
		int res = 7;
		glm::vec2 size(1000.0f);

		{
			ResourceManager* RscManager = Engine::GetModule<ResourceManager>();

			std::string modelPath_1 = "assets/models/Lantern/lantern_obj_1.obj";
			RscManager->LoadResource<Mesh>(modelPath_1);
			Ptr<Mesh> lanternModel_1 = RscManager->GetResource<Mesh>(modelPath_1);

			std::string modelPath_2 = "assets/models/Lantern/lantern_obj_2.obj";
			RscManager->LoadResource<Mesh>(modelPath_2);
			Ptr<Mesh> lanternModel_2 = RscManager->GetResource<Mesh>(modelPath_2);

			std::string tex0 = "assets/models/Lantern/lantern_Base_Color.jpg";
			std::string tex1 = "assets/models/Lantern/lantern_Mixed_AO.jpg";
			std::string tex2 = "assets/models/Lantern/lantern_Metallic.jpg";
			std::string tex3 = "assets/models/Lantern/lantern_Roughness.jpg";
			RscManager->LoadResource<Texture2D>(tex0);
			RscManager->LoadResource<Texture2D>(tex1);
			RscManager->LoadResource<Texture2D>(tex2);
			RscManager->LoadResource<Texture2D>(tex3);

			RscManager->GetResource<Texture2D>(tex0)->renderRscTexture = new RenderRscTexture();
			RscManager->GetResource<Texture2D>(tex0)->LoadOnGpu();

			RscManager->GetResource<Texture2D>(tex1)->renderRscTexture = new RenderRscTexture();
			RscManager->GetResource<Texture2D>(tex1)->LoadOnGpu();

			RscManager->GetResource<Texture2D>(tex2)->renderRscTexture = new RenderRscTexture();
			RscManager->GetResource<Texture2D>(tex2)->LoadOnGpu();

			RscManager->GetResource<Texture2D>(tex3)->renderRscTexture = new RenderRscTexture();
			RscManager->GetResource<Texture2D>(tex3)->LoadOnGpu();

			Ptr<Material> mat_0(new Material(basicMatShader_1));
			mat_0->SetTexture("baseColorTexture", RscManager->GetResource<Texture2D>(tex0).get());
			mat_0->SetTexture("AOTexture", RscManager->GetResource<Texture2D>(tex1).get());
			mat_0->SetTexture("metallicTexture", RscManager->GetResource<Texture2D>(tex2).get());
			mat_0->SetTexture("roughnessTexture", RscManager->GetResource<Texture2D>(tex3).get());
			mat_0->SetColor("color", glm::vec4(1.0f, 0.1, 0.1, 1.0));
			mat_0->SetColor("emission", glm::vec4(0.0f));
			mat_0->SetScalar("roughness", 0.5f);
			mat_0->SetScalar("metallic", 1.0f);
			mat_0->SetScalar("specular", 1.0f);
			mat_0->LoadOnGpu();


			Ptr<Material> mat_2(new Material(basicMatShader_2));
			mat_2->SetTexture("baseColorTexture", RscManager->GetResource<Texture2D>(tex0).get());
			mat_2->SetTexture("AOTexture", RscManager->GetResource<Texture2D>(tex1).get());
			mat_2->SetTexture("metallicTexture", RscManager->GetResource<Texture2D>(tex2).get());
			mat_2->SetTexture("roughnessTexture", RscManager->GetResource<Texture2D>(tex3).get());
			mat_2->SetColor("color", glm::vec4(1.0f));
			mat_2->SetColor("emission", glm::vec4(0.0f));
			mat_2->SetScalar("roughness", 1.0f);
			mat_2->SetScalar("metallic", 1.0f);
			mat_2->SetScalar("specular", 22.0f);
			mat_2->SetScalar("alpha", 0.75f);
			mat_2->LoadOnGpu();


			{
				auto meshEntity = newScene->CreateEntity("Lantern");
				auto& tr = meshEntity.GetOrAddComponent<Transform>();
				tr.SetLocalPosition(glm::vec3(0.0f, -4.0f, 0.0f));
				tr.SetLocalScale(glm::vec3(0.1f));

				auto& model = meshEntity.GetOrAddComponent<Model>();
				model.AddMesh(lanternModel_1);
				model.AddMesh(lanternModel_2);
				model.SetMaterial(0, mat_0);
				model.SetMaterial(1, mat_2);
			}



		}


		{
			Ptr<Mesh> plane = Ptr<Mesh>( MeshFactory::CreateQuad() );
			
			auto meshEntity = newScene->CreateEntity("Plane");
			auto& tr = meshEntity.GetOrAddComponent<Transform>();
			tr.SetLocalPosition(glm::vec3(0.0f, -4.3f, 0.0f));
			tr.SetLocalScale(glm::vec3(3000.0f));
			tr.SetLocalOrientation(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f));
			
			auto& model = meshEntity.GetOrAddComponent<Model>();
			model.AddMesh(plane);
		}


		{
			glm::vec4 lightPositions[4] = {
				 glm::vec4( 4.0f, 9.0f, 4.0f, 1.0f),
				 glm::vec4(-4.0f, 9.0f, 4.0f, 1.0f),
				 glm::vec4(-4.0f, 9.0f,-4.0f, 1.0f),
				 glm::vec4( 4.0f, 9.0f,-4.0f, 1.0f),
			};

			for (int32_t i = 0; i < 4; ++i)
			{
				auto& lightEntity = newScene->CreateEntity("Lights");

				auto& lightComp = lightEntity.GetOrAddComponent<Light>();
				lightComp.type = (int32_t)LightType::PointLight;
				lightComp.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				lightComp.intensity = 370.0f;
				lightComp.radius = 17.0f;
				lightComp.clipDistance = 20.0f;

				auto& tr = lightEntity.GetOrAddComponent<Transform>();
				tr.SetLocalPosition(lightPositions[i]);
			}


			for (int32_t i = 0; i < 4; ++i)
			{
				auto& lightEntity = newScene->CreateEntity("Lights");

				auto& lightComp = lightEntity.GetOrAddComponent<Light>();
				lightComp.type = (int32_t)LightType::PointLight;
				lightComp.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				lightComp.intensity = 70.0f;
				lightComp.radius = 17.0f;
				lightComp.clipDistance = 5000.0f;

				auto& tr = lightEntity.GetOrAddComponent<Transform>();
				tr.SetLocalPosition(lightPositions[i] + glm::vec4(700.0f));
			}

		}
		

		// Switch the scene....
		uint32_t sceneIdx = GetModule<SceneManager>()->AddScene(newScene);
		// load scene using its id
		GetModule<SceneManager>()->SwitchScene(sceneIdx);
	}
};
