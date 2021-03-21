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

	void Scene::Save(const std::string& filePath, bool binary)
	{
		// loop through the scene and serialize each entity and its components

	}

	void Scene::Load(const std::string& filePath, bool binary)
	{
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
