//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <memory>
#include "Utilities/Core.h"
#include "ISystem.h"

namespace Raven
{
	class Scene;
	class SystemManager final
	{
	public:
		template<typename T, typename... Args>
		std::shared_ptr<T> AddSystem(Args&&... args)
		{
			auto typeName = typeid(T).hash_code();
			RAVEN_ASSERT(systems.find(typeName) == systems.end(), "Add system more than once.");
			return systems.emplace(typeName, std::make_shared<T>(std::forward<Args>(args)...)).first->second;
		}

		template<typename T>
		std::shared_ptr<T> AddSystem(T* t)
		{
			auto typeName = typeid(T).hash_code();
			RAVEN_ASSERT(systems.find(typeName) == systems.end(), "Add system more than once.");
			return systems.emplace(typeName, std::shared_ptr<T>(t)).first->second;
		}

		template<typename T>
		void RemoveSystem()
		{
			auto typeName = typeid(T).hash_code();

			if (systems.find(typeName) != systems.end())
			{
				systems.erase(typeName);
			}
		}

		template<typename T>
		T* GetSystem()
		{
			auto typeName = typeid(T).hash_code();
			if (systems.find(typeName) != systems.end())
			{
				return dynamic_cast<T*>(systems[typeName].get());
			}
			return nullptr;
		}

		template<typename T>
		T* HasSystem()
		{
			auto typeName = typeid(T).hash_code();
			return systems.find(typeName) != systems.end();
		}

		void OnUpdate(float dt, Scene* scene);
		void OnImGui();

	private:
		std::unordered_map<size_t, std::shared_ptr<ISystem>> systems;
	};
}
