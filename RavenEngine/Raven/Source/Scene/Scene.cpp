//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "Scene.h"
#include "Entity/EntityManager.h"

namespace Raven { 

	Scene::Scene(const std::string& initName)
		:name(initName)
	{
		entityManager = std::make_shared<EntityManager>(this);
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

	void Scene::Serialize(const std::string& filePath, bool binary)
	{
	}

	void Scene::Load(const std::string& filePath, bool binary)
	{
	}

	void Scene::CreateEntity()
	{
		entityManager->Create();
	}

	void Scene::OnInit()
	{
		LOGV("{0}", __FUNCTION__);
	}

	void Scene::OnClean()
	{
	}

	void Scene::OnUpdate(float dt)
	{
	}

};
