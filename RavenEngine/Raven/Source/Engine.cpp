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

		auto& lightEntity = newScene->CreateEntity("sun");
		auto& lightComp = lightEntity.GetOrAddComponent<Light>();
		lightComp.type = (int32_t)LightType::DirectionalLight;
		lightComp.direction = glm::normalize(glm::vec3(-1.0f));
		lightComp.intensity = 1.0f;


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


		glm::vec4 randomColors[8] = {
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0),
		};


		//
		int res = 25;
		glm::vec2 size(1000.0f);
		Ptr<Mesh> sphereMesh(MeshFactory::CreateSphere());

		for (int32_t x = 0; x < res; ++x)
		{
			for (int32_t y = 0; y < res; ++y)
			{
				float fx = (float)x / (float)(res - 1);
				float fy = (float)y / (float)(res - 1);

				//
				glm::vec3 pos = glm::vec3(size.x * fx, 100.0f, size.y * fy);

				auto meshEntity = newScene->CreateEntity("Model_" + std::to_string(x+y*res));
				auto& tr = meshEntity.GetOrAddComponent<Transform>();
				tr.SetLocalPosition(pos);
				tr.SetLocalScale(glm::vec3(10.0f));


				Ptr<Material> basicMat(new Material(basicMatShader));
				basicMat->SetColor("color", randomColors[rand() % 8]);

				glm::vec4 matColor;
				basicMat->GetColor("color", matColor);

				basicMat->SetColor("emission", matColor);
				basicMat->SetScalar("roughness", 0.0f);
				basicMat->SetScalar("metallic", 0.0f);
				basicMat->SetScalar("specular", 1.0f);
				basicMat->LoadOnGpu();

				auto& model = meshEntity.GetOrAddComponent<Model>();
				model.AddMesh(sphereMesh);
				model.SetMaterial(0, basicMat);
			}
		}


		// Switch the scene....
		uint32_t sceneIdx = GetModule<SceneManager>()->AddScene(newScene);
		GetModule<SceneManager>()->SwitchScene(sceneIdx);
	}
};
