//
#include <memory>


#include <GLFW/glfw3.h>


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
#include "Scene/System/SystemManager.h"
#include "Scene/System/GUISystem.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Entity/Entity.h"

#include "GUI/GUIModule.h"

#include "ProceduralGenerator/TerrainGeneration.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/Resources/MaterialShader.h"
#include "ResourceManager/Resources/Material.h"


#include "Devices/Input.h"

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
		eventDispatcher.DispatchEvents();
		systemManager->OnUpdate(dt, GetModule<Raven::SceneManager>()->GetCurrentScene());
		GetModule<Raven::SceneManager>()->GetCurrentScene()->OnUpdate(dt);

		// Update Render...
		GetModule<RenderModule>()->Update(dt);




		// ~TESTING---------------------------------------------------------------------------------------------
		if (TESTING_MAT)
		{
			static float roughnessTime = 0.0f;
			roughnessTime += dt;
			TESTING_MAT->SetScalar("roughness", glm::abs(glm::sin(roughnessTime * 0.4f)));
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
		guiSystem->OnInit();
	
//############	Register your system here ######################

	}

	void Engine::DestoryModules()
	{
		// Destroy - Here order matter.
		DestroyModule<TerrainGeneration>();
		DestroyModule<LuaVirtualMachine>();
		DestroyModule<RenderModule>();
		DestroyModule<SceneManager>();
		DestroyModule<GUIModule>();
		DestroyModule<ImGuiEngine>();
		DestroyModule<ResourceManager>();
		DestroyModule<Window>();
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
		generator->Noise(100, 100, TerrainGeneration::FileFormat::PNG);
		std::string path("heightmap.png");
		// load the generated height map into resource manager
		GetModule<ResourceManager>()->LoadResource<Texture2D>(path);

		Terrain* terrain = new Terrain(GetModule<ResourceManager>()->GetResource<Texture2D>(path).get());

		std::cout << terrain->heightMap->width << '\n';
		std::cout << terrain->heightMap->height << " \n";

		Scene* newScene = new Scene("TerrainScene");
		newScene->dynamic = true;

		auto terrainEntity = newScene->CreateEntity("terrain");
		terrainEntity.AddComponent<TerrainComponent>(path, std::shared_ptr<Terrain>(terrain));

		{
			auto& lightEntity = newScene->CreateEntity("THE_SUN");
			auto& lightComp = lightEntity.GetOrAddComponent<Light>();
			auto& tr = lightEntity.GetOrAddComponent<Transform>();

			lightComp.type = (int32_t)LightType::DirectionalLight;
			lightComp.color = glm::vec4(1.0f, 0.95f, 0.8f, 1.0f);
			lightComp.intensity = 1.2f;
			lightComp.direction = glm::normalize(glm::vec3(-1.0f));
		}


		// Basic Material...
		Ptr<MaterialShader> basicMatShader( new MaterialShader() );
		basicMatShader->SetDomain(ERenderShaderDomain::Mesh);
		basicMatShader->SetType(ERenderShaderType::Opaque);
		basicMatShader->SetMaterialFunction("shaders/Materials/BasicMaterial.glsl");
		basicMatShader->GetBlockInput().BeginUniformBlock("MaterialParamtersBlock");
		basicMatShader->GetBlockInput().AddInput(EShaderInputType::Vec4, "color");
		basicMatShader->GetBlockInput().AddInput(EShaderInputType::Vec4, "emission");
		basicMatShader->GetBlockInput().AddInput(EShaderInputType::Float, "roughness");
		basicMatShader->GetBlockInput().AddInput(EShaderInputType::Float, "metallic");
		basicMatShader->GetBlockInput().AddInput(EShaderInputType::Float, "specular");
		basicMatShader->GetBlockInput().EndUniformBlock();
		basicMatShader->AddSampler("colorTexture");
		basicMatShader->LoadOnGpu();


		//
		int res = 7;
		glm::vec2 size(1000.0f);
		Ptr<Mesh> sphereMesh(MeshFactory::CreateSphere());

		{
			glm::vec3 pos = glm::vec3(size.x * 0.5, 220.0f, size.y * 0.5);

			auto meshEntity = newScene->CreateEntity("Middle_Sphere");
			auto& tr = meshEntity.GetOrAddComponent<Transform>();
			tr.SetLocalPosition(pos);
			tr.SetLocalScale(glm::vec3(150.0f));


			TESTING_MAT = Ptr<Material>(new Material(basicMatShader));
			TESTING_MAT->SetColor("color", glm::vec4(0.1));
			TESTING_MAT->SetColor("emission", glm::vec4(0.0));
			TESTING_MAT->SetScalar("roughness", 0.05f);
			TESTING_MAT->SetScalar("metallic", 1.0f);
			TESTING_MAT->SetScalar("specular", 1.0f);
			TESTING_MAT->LoadOnGpu();

			auto& model = meshEntity.GetOrAddComponent<Model>();
			model.AddMesh(sphereMesh);
			model.SetMaterial(0, TESTING_MAT);
		}
		


		glm::vec4 randomColors[8] = {
			glm::vec4(0.1f, 0.1f, 0.1f, 1.0),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0),
		};




		for (int32_t x = 0; x < res; ++x)
		{

			for (int32_t y = 0; y < res; ++y)
			{
				if (!(x == 0 || x == res - 1 || y == 0 || y == res - 1))
					continue;

				float fx = (float)x / (float)(res - 1);
				float fy = (float)y / (float)(res - 1);

				//
				glm::vec3 pos = glm::vec3(size.x * fx, 200.0f, size.y * fy);

				auto meshEntity = newScene->CreateEntity("Model_" + std::to_string(x+y*res));
				auto& tr = meshEntity.GetOrAddComponent<Transform>();
				tr.SetLocalPosition(pos);
				tr.SetLocalScale(glm::vec3(80.0f));


				Ptr<Material> basicMat(new Material(basicMatShader));
				basicMat->SetColor("color", glm::vec4(0.4f));
				basicMat->SetColor("emission", glm::vec4(0.0f));
				basicMat->SetScalar("roughness", fx);
				basicMat->SetScalar("metallic", fy);
				basicMat->SetScalar("specular", 1.0f);
				basicMat->LoadOnGpu();

				auto& model = meshEntity.GetOrAddComponent<Model>();
				model.AddMesh(sphereMesh);
				model.SetMaterial(0, basicMat);
			}
		}



		for (int32_t i = 0; i < 16; ++i)
		{
			float f = (float)i / (float)(16 - 1);
			f *= 6.15132f;

			auto& lightEntity = newScene->CreateEntity("light");
			auto& lightComp = lightEntity.GetOrAddComponent<Light>();
			lightComp.type = (int32_t)LightType::PointLight;
			lightComp.intensity = 40000.0f;
			lightComp.radius = 450.0f;
			lightComp.position = glm::vec3(cos(f), 0.4, sin(f)) * 500.0f + glm::vec3(500.0f, 150.0f, 500.0f);

			lightComp.direction = glm::normalize(glm::vec3(500.0f, 100.0f, 500.0f) - lightComp.position);

			lightComp.color = glm::vec4(randomColors[i % 8]);
			lightComp.innerAngle = 20.0f;
			lightComp.outerAngle = 25.0f;

			auto& lightTransfrom = lightEntity.GetOrAddComponent<Transform>();
			lightTransfrom.SetLocalPosition(lightComp.position);

		}

		// Switch the scene....
		uint32_t sceneIdx = GetModule<SceneManager>()->AddScene(newScene);
		GetModule<SceneManager>()->SwitchScene(sceneIdx);
	}
};
