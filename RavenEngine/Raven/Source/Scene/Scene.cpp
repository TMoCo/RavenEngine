//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "Scene.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "SceneGraph.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Component/CameraControllerComponent.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/MeshRenderer.h"
#include "Scene/Component/RigidBody.h"

#include "Scripts/LuaComponent.h"
#include "Core/CameraController.h"

#include "Render/RenderModule.h"
#include "Render/RenderTarget.h"

#include "Utilities/StringUtils.h"
#include "Core/Camera.h"
#include "Devices/Input.h"

#include "Animation/Animator.h"

#include <fstream>
#include "Engine.h"
#include "Utilities/Serialization.h"
// for serialization
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"

#include <glm/gtx/string_cast.hpp>



namespace Raven { 

	Scene::Scene(const std::string& initName)
		:name(initName)
	{
		LOGV("{0} {1}", __FUNCTION__,initName);
		entityManager = std::make_shared<EntityManager>(this);

		entityManager->AddDependency<Camera, Transform>();
		entityManager->AddDependency<Model, Transform>();
		entityManager->AddDependency<Light, Transform>();
		entityManager->AddDependency<MeshRenderer, Transform>();
		entityManager->AddDependency<RigidBody, Transform>();

		sceneGraph = std::make_shared<SceneGraph>();
		sceneGraph->Init(entityManager->GetRegistry());
	}

	entt::registry& Scene::GetRegistry()
	{
		return entityManager->GetRegistry();
	}

	void Scene::SetSize(uint32_t w, uint32_t h)
	{
		width = w;
		height = h;
	}

#define ALL_COMPONENTS Transform, NameComponent, ActiveComponent, Hierarchy, Camera, Light, CameraControllerComponent, Model, LuaComponent, MeshRenderer, SkinnedMeshRenderer, Animator, RigidBody

	void Scene::Save(const std::string& filePath, bool binary)
	{
		PRINT_FUNC();
		// loop through the scene and serialize each entity and its components
		std::string path = filePath + name;
		if (binary)
		{
			/*path += std::string(".bin");

			std::ofstream file(path, std::ios::binary);

			{
				// output finishes flushing its contents when it goes out of scope
				cereal::BinaryOutputAr chive output{ file };
				output(*this);
				entt::snapshot{ entityManager->GetRegistry() }.entities(output).component<ALL_COMPONENTS>(output);
			}
			file.flush();
			file.close();*/
		}
		else
		{
			std::stringstream storage;
			path += std::string(".raven");

			{
				cereal::JSONOutputArchive output{ storage };
				output(*this);
				entt::snapshot{ entityManager->GetRegistry() }.entities(output).component<ALL_COMPONENTS>(output);				
			}

			std::ofstream file(path, std::ios::binary);

			file << storage.str();

			file.flush();
			file.close();
		}
	}

	void Scene::Load(const std::string& filePath, bool binary)
	{
		PRINT_FUNC();

		entityManager->Clear();
		sceneGraph->DisconnectOnConstruct(true, entityManager->GetRegistry());
		std::string path = filePath + name;

		if (binary)
		{
			/*path += std::string(".bin");
			std::ifstream file(path, std::ios::binary);
			if (!file.good())
			{
				LOGE("No saved scene file found {0}", path);
				return;
			}

			cereal::BinaryInputArchive input(file);
			input(*this);
	
			entt::snapshot_loader{ entityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTS>(input);*/
		}
		else
		{
			path += std::string(".raven");

			std::ifstream in(path);
			if (in.good())
			{
				std::string data;
				in.seekg(0, std::ios::end);
				auto len = in.tellg();
				in.seekg(0, std::ios::beg);
				data.resize(len);
				in.read(data.data(), len);
				in.close();

				std::istringstream istr;
				istr.str(data);
				cereal::JSONInputArchive input(istr);
				input(*this);
				entt::snapshot_loader{ entityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTS>(input);
			}
			else
			{
				LOGE("No saved scene file found {0}", path);
				in.close();
			}
		}
		sceneGraph->DisconnectOnConstruct(false,entityManager->GetRegistry());
		
	}

	Raven::Entity Scene::CreateEntity()
	{
		return entityManager->Create();
	}

	Raven::Entity Scene::CreateEntity(const std::string& name)
	{
		return entityManager->Create(name);
	}

	void Scene::DuplicateEntity(const Entity& entity, const Entity& parent)
	{
		Entity newEntity = entityManager->Create();
		
		if (parent)
			newEntity.SetParent(parent);

		CopyComponents(entity, newEntity);
	}

	void Scene::DuplicateEntity(const Entity& entity)
	{
		Entity newEntity = entityManager->Create();
		//COPY
		CopyComponents(entity,newEntity);
	}

	Camera* Scene::GetTargetCamera()
	{
		auto camsEttView = entityManager->GetEntitiesWithType<Camera>();
		if ((!camsEttView.Empty() && Engine::Get().GetEditorState() == EditorState::Play) || forceShow)
		{
			Camera& sceneCam = camsEttView.Front().GetComponent<Camera>();
			return &sceneCam;
		}

		return overrideCamera;
	}

	Raven::Transform* Scene::GetCameraTransform()
	{
		auto camsEttView = entityManager->GetEntitiesWithType<Camera>();
		if ((!camsEttView.Empty() && Engine::Get().GetEditorState() == EditorState::Play) || forceShow)
		{
			Transform& sceneCamTr = camsEttView[0].GetComponent<Transform>();
			return &sceneCamTr;
		}

		return overrideTransform;
	}

	

	void Scene::CopyComponents(const Entity& from, const Entity& to)
	{
		LOGW("Not implementation {0}", __FUNCTION__);
	}

	void Scene::OnInit()
	{
		if (initCallback != nullptr) 
		{
			initCallback(this);
		}
		auto luaView = GetRegistry().view<LuaComponent>();
		for (auto v : luaView)
		{
			auto& lua = GetRegistry().get<LuaComponent>(v);
			lua.OnInit();
		}
		// on scene init, we need to initialise the physics engine
		auto view = entityManager->GetRegistry().view<RigidBody>();
		for (auto v : view)
		{
			auto& rb = entityManager->GetRegistry().get<RigidBody>(v);
			// initialise the body
			rb.InitRigidBody();
			// initialise the start transform in the rigid body engine to be the entity's current transform
			rb.SetInitTransform(GetRegistry().get<Transform>(v)); // rigid body needs transform to exist so this should never fail
			rb.InitTransform();
			// initialise the colliders attached to the body (if any)
			for (auto& collider : *rb.GetAllColliders())
			{
				collider->SetBody(rb.GetBody());
				collider->InitShape(Engine::Get().GetModule<PhysicsModule>()->GetPhysicsCommon());
			}
		}
	}

	void Scene::OnClean()
	{
		LOGE("CLEANING");
	}

	auto Scene::UpdateCameraController(float dt)
	{
		// Update AspectRatio of the camera to match render target.
		if (GetTargetCamera())
		{
			float rtRatio = Engine::GetModule<RenderModule>()->GetSceneRT()->GetAspectRatio();
			GetTargetCamera()->SetAspectRatio(rtRatio);
		}

		auto controller = entityManager->GetRegistry().group<CameraControllerComponent>(entt::get<Transform>);
		for (auto entity : controller)
		{
			const auto mousePos = Input::GetInput()->GetMousePosition();
			auto& [con, trans] = controller.get<CameraControllerComponent, Transform>(entity);
			if (Engine::Get().IsSceneActive() && 
				Engine::Get().GetEditorState() == EditorState::Play&&
				con.GetController() )
			{
				con.GetController()->HandleMouse(trans, dt, mousePos.x, mousePos.y);
				con.GetController()->HandleKeyboard(trans, dt);
			}
		}
	}

	void Scene::OnUpdate(float dt)
	{
		UpdateCameraController(dt);
		sceneGraph->Update(entityManager->GetRegistry());
	}

};
