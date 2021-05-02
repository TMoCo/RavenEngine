//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "EntityManager.h"

namespace Raven
{
	Entity EntityManager::Create()
	{
		return Entity(registry.create(), scene);
	}

	Entity EntityManager::Create(const std::string& name)
	{
		auto e = registry.create();
		Entity entity(e, scene);
		entity.AddComponent<NameComponent>(name);
		return entity;
	}

	void EntityManager::Clear()
	{
		registry.each([&](auto entity) {
			registry.destroy(entity);
		});
		registry.clear();
	}

	Entity EntityManager::GetEntityByName(const std::string& name)
	{
		auto views = registry.view<NameComponent>();
		for (auto & view : views) 
		{
			auto & comp = registry.get<NameComponent>(view);
			if (comp.name == name) {
				return { view,scene };
			}
		}
		return { entt::null,nullptr };
	}

};
