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
#include "ResourceManager/MeshFactory.h"
#include "ResourceManager/Resources/Terrain.h"

#include "ImGui/ImGuiEngine.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/System/SystemManager.h"
#include "Scene/System/GUISystem.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Component/MeshComponent.h"
#include "Scene/Component/SkinnedMeshComponent.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Entity/Entity.h"

#include "GUI/GUIModule.h"

#include "ProceduralGenerator/TerrainGeneration.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/Resources/SkinnedMesh.h"
#include "ResourceManager/Resources/MaterialShader.h"
#include "ResourceManager/Resources/Material.h"
#include "ResourceManager/Importers/FBXImporter.h"


#include "Devices/Input.h"
#include "Scripts/LuaSystem.h"
#include "Animation/AnimationSystem.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "Animation/Skeleton.h"
#include "Scripts/LuaVirtualMachine.h"





namespace Raven 
{

// Create Singleton instance.
Engine* Engine::instance = CreateEngine();





Engine::Engine()
	: engineTime(0.0f)
{

}

Engine::~Engine()
{

}


void Engine::Initialize()
{
	// Module
	LoadModules();

}


	int Engine::Run()
	{
		auto win = GetModule<Raven::Window>();


		NewGameScene();


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
		Scene* newScene = new Scene("TerrainScene");
		newScene->dynamic = true;


		{
			if (!Engine::GetModule<ResourceManager>()->AddResrouce("MESH_lantern.raven"))
			{
				GetModule<ResourceManager>()->Import("./assets/models/Lantern/lantern.obj");
			}

			{
				Ptr<Mesh> mesh = GetModule<ResourceManager>()->GetResource<Mesh>("MESH_lantern");

				auto meshEntity = newScene->CreateEntity("MESH_lantern");
				auto& tr = meshEntity.GetOrAddComponent<Transform>();
				tr.SetPosition(glm::vec3(0.0f, 20.0f, 0.0f));
				tr.SetScale(glm::vec3(0.5f));

				auto& model0 = meshEntity.GetOrAddComponent<MeshComponent>();
				model0.SetMesh(mesh);
			}


			if (!Engine::GetModule<ResourceManager>()->AddResrouce("SKINNEDMESH_ybot.raven"))
			{
				GetModule<ResourceManager>()->Import("./assets/ybot.fbx");
			}
			else
			{
				Engine::GetModule<ResourceManager>()->AddResrouce("SKELETON_ybot.raven");
				Engine::GetModule<ResourceManager>()->AddResrouce("ANIM_CLIP_ybot_0.raven");
			}

			Ptr<SkinnedMesh> skinnedMesh = GetModule<ResourceManager>()->GetResource<SkinnedMesh>("SKINNEDMESH_ybot");


			if (!Engine::GetModule<ResourceManager>()->AddResrouce("ANIM_CLIP_Idle_0.raven"))
			{
				GetModule<ResourceManager>()->GetImporter<FBXImporter>()->settings.importAnimationOnly = true;
				GetModule<ResourceManager>()->GetImporter<FBXImporter>()->settings.skeleton = skinnedMesh->GetSkeleton();
				GetModule<ResourceManager>()->Import("./assets/Idle.fbx");
			}


			if (!Engine::GetModule<ResourceManager>()->AddResrouce("ANIM_CLIP_Walking_0.raven"))
			{
				GetModule<ResourceManager>()->GetImporter<FBXImporter>()->settings.importAnimationOnly = true;
				GetModule<ResourceManager>()->GetImporter<FBXImporter>()->settings.skeleton = skinnedMesh->GetSkeleton();
				GetModule<ResourceManager>()->Import("./assets/Walking.fbx");
			}


			std::vector< Ptr<AnimationClip> > clip;
			clip.push_back(GetModule<ResourceManager>()->GetResource<AnimationClip>("ANIM_CLIP_Idle_0.raven"));
			clip.push_back(GetModule<ResourceManager>()->GetResource<AnimationClip>("ANIM_CLIP_Walking_0.raven"));

			int32_t num = 5;

			for (int32_t i = 0; i < num * num; ++i)
			{
				float f = (float)i / (float)(num * num);
				float fx = (float)(i / num) / (float)(num * num);
				float fy = (float)(i % num) / (float)(num * num);

				auto meshEntity = newScene->CreateEntity("FBX_MODEL_" + std::to_string(i));
				auto& tr = meshEntity.GetOrAddComponent<Transform>();
				tr.SetPosition(glm::vec3(fx * 45.0f * num, -4.0f, fy * 45.0f * num));
				tr.SetScale(glm::vec3(0.1f));

				auto& model0 = meshEntity.GetOrAddComponent<SkinnedMeshComponent>();
				model0.SetMesh(skinnedMesh);
				model0.SetMaterial(0, nullptr);

				Ptr<Animation> anim(new Animation());

				if (i > (num * num / 2))
				{
					anim->AddClip(clip[0]);
				}
				else
				{
					anim->AddClip(clip[1]);
				}


				auto& modelAnimator = meshEntity.GetOrAddComponent<Animator>();
				modelAnimator.isSimpleAnimator = true;
				modelAnimator.anim = anim;
				modelAnimator.offset = f * 4.0f;
			}


		}

		// Switch the scene....
		uint32_t sceneIdx = GetModule<SceneManager>()->AddScene(newScene);
		GetModule<SceneManager>()->SwitchScene(sceneIdx);
	}
};
