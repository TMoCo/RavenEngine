//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include <string>
#include "EntityGroup.h"

namespace Raven
{
	class Scene;

	class EntityManager final
	{
	public:
		EntityManager(Scene* initScene) : scene(initScene)
		{
		};

		Entity Create();
		Entity Create(const std::string& name);

		template<typename... Components>
		auto GetEntitiesWithTypes()
		{
			return registry.group<Components...>();
		}

		template<typename Component>
		EntityView<Component> GetEntitiesWithType()
		{
			return EntityView<Component>(scene);
		}


		Entity GetEntityByName(const std::string& name);

		template<typename R, typename T>
		void AddDependency();

		inline auto& GetRegistry(){ return registry; }

		void Clear();

	private:
		Scene* scene = nullptr;
		entt::registry registry;
	};

	template<typename R, typename T>
	void EntityManager::AddDependency()
	{
		registry.on_construct<R>().connect<&entt::registry::get_or_emplace<T>>();
	}

};