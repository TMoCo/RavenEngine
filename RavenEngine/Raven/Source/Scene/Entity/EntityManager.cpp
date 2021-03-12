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
		registry.emplace<NameComponent>(e, name);
		return Entity(e, scene);
	}

	void EntityManager::Clear()
	{
		registry.each([&](auto entity) {
			registry.destroy(entity);
		});
		registry.clear();
	}

};