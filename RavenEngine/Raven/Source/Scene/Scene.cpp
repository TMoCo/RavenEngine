//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "Scene.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "SceneGraph.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "ResourceManager/Resources/Model.h"
#include "Utilities/StringUtils.h"
#include "Core/Camera.h"

#include <fstream>

// for serialization
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"

namespace Raven { 

	Scene::Scene(const std::string& initName)
		:name(initName)
	{
	
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

#define ALL_COMPONENTS Transform, NameComponent, ActiveComponent, Hierarchy, Camera, Light

	void Scene::Save(const std::string& filePath, bool binary)
	{
		// loop through the scene and serialize each entity and its components
	/*	std::string path = filePath + name;
		if (binary)
		{
			path += std::string(".bin");

			std::ofstream file(path, std::ios::binary);

			{
				// output finishes flushing its contents when it goes out of scope
				cereal::BinaryOutputArchive output{ file };
				output(*this);
				entt::snapshot{ entityManager->GetRegistry() }.entities(output).component<ALL_COMPONENTS>(output);
			}
			file.close();
		}
		else
		{
			std::stringstream storage;
			path += std::string(".lsn");

			{
				cereal::JSONOutputArchive output{ storage };
				output(*this);
				entt::snapshot{ entityManager->GetRegistry() }.entities(output).component<ALL_COMPONENTS>(output);
				
			}

			FileSystem::Write(path, storage.str());
		}*/
	}

	void Scene::Load(const std::string& filePath, bool binary)
	{

		entityManager->Clear();
		sceneGraph->DisconnectOnConstruct(true, entityManager->GetRegistry());
		std::string path = filePath + name;

		if (binary)
		{
			path += std::string(".bin");

			/*if (!FileSystem::FileExists(path))
			{
				LOGE("No saved scene file found {0}", path);
				return;
			}

			std::ifstream file(path, std::ios::binary);
			cereal::BinaryInputArchive input(file);
			input(*this);
	
			entt::snapshot_loader{ entityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTS>(input);*/
		}
		else
		{
			path += std::string(".raven");

	/*		if (!FileSystem::FileExists(path))
			{
				LOGE("No saved scene file found {0}", path);
				return;
			}
			std::string data = FileSystem::Read(path);
			std::istringstream istr;
			istr.str(data);
			cereal::JSONInputArchive input(istr);
			input(*this);

			entt::snapshot_loader{ entityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTS>(input);*/
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

	void Scene::CopyComponents(const Entity& from, const Entity& to)
	{
		LOGW("Not implementation {0}", __FUNCTION__);
	}

	void Scene::OnInit()
	{
		LOGV("{0}", __FUNCTION__);
		entityManager = std::make_shared<EntityManager>(this);
		
		entityManager->AddDependency<Camera, Transform>();
		entityManager->AddDependency<Model, Transform>();
		entityManager->AddDependency<Light, Transform>();
		
		sceneGraph = std::make_shared<SceneGraph>();
		sceneGraph->Init(entityManager->GetRegistry());

	}

	void Scene::OnClean()
	{

	}

	void Scene::OnUpdate(float dt)
	{
		sceneGraph->Update(entityManager->GetRegistry());
	}

};
