//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Scene/Scene.h"
#include "Scene/SceneGraph.h"
#include "Scene/Component/Component.h"

namespace Raven
{
	class Scene;
	class Entity
	{
	public:

		Entity() = default;

		Entity(entt::entity handle, Scene* s)
			: entityHandle(handle)
			, scene(scene)
		{
		}

		~Entity()
		{
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
#ifdef RAVEN_DEBUG
			if (HasComponent<T>())
				LOGW("Attempting to add extisting component ");
#endif
			return scene->GetRegistry().emplace<T>(entityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		T& GetOrAddComponent(Args&&... args)
		{
			return scene->GetRegistry().get_or_emplace<T>(entityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		void AddOrReplaceComponent(Args&&... args)
		{
			scene->GetRegistry().emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return scene->GetRegistry().get<T>(entityHandle);
		}

		template<typename T>
		T* TryGetComponent()
		{
			return scene->GetRegistry().try_get<T>(entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return scene->GetRegistry().has<T>(entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			return scene->GetRegistry().remove<T>(entityHandle);
		}

		bool IsActive();
		void SetActive(bool isActive);
		void SetParent(const Entity& entity);
		Entity GetParent();
		std::vector<Entity> GetChildren();
		bool IsParent(Entity potentialParent);

		inline operator entt::entity() const {return entityHandle; }
		inline operator uint32_t() const { return (uint32_t)entityHandle; }
		inline operator bool() const {	return entityHandle != entt::null && scene;}

		inline auto operator==(const Entity& other) const {return entityHandle == other.entityHandle && scene == other.scene;}
		inline auto operator!=(const Entity& other) const{return !(*this == other); }

		inline auto GetHandle() const{ return entityHandle; }

		void Destroy();
		bool Valid();
		
	private:
		entt::entity entityHandle = entt::null;
		Scene * scene = nullptr;
		friend class EntityManager;
	};
}
