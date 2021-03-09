//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "Entity.h"

namespace Raven
{
	bool Entity::IsActive()
	{
		if (HasComponent<ActiveComponent>())
			return scene->GetRegistry().get<ActiveComponent>(entityHandle).active;

		return true;
	}

	void Entity::SetActive(bool isActive)
	{
		GetOrAddComponent<ActiveComponent>().active = isActive;
	}

	void Entity::SetParent(const Entity& entity)
	{
		bool acceptable = false;
		auto hierarchyComponent = TryGetComponent<Hierarchy>();
		if (hierarchyComponent != nullptr)
		{
			acceptable = entity.entityHandle != entityHandle && (!entity.IsParent(*this)) && (hierarchyComponent->Parent() != entityHandle);
		}
		else
			acceptable = entity.entityHandle != entityHandle;

		if (!acceptable)
			return;

		if (hierarchyComponent)
			Hierarchy::Reparent(entityHandle, entity.entityHandle, scene->GetRegistry(), *hierarchyComponent);
		else
		{
			scene->GetRegistry().emplace<Hierarchy>(entityHandle, entity.entityHandle);
		}
	}

	Entity Entity::GetParent()
	{
		auto hierarchyComp = TryGetComponent<Hierarchy>();
		if (hierarchyComp)
			return Entity(hierarchyComp->Parent(), scene);
		else
			return Entity(entt::null, nullptr);
	}

	std::vector<Entity> Entity::GetChildren()
	{
		std::vector<Entity> children;
		auto hierarchyComponent = TryGetComponent<Hierarchy>();
		if (hierarchyComponent)
		{
			entt::entity child = hierarchyComponent->First();
			while (child != entt::null && scene->GetRegistry().valid(child))
			{
				children.emplace_back(child, scene);
				hierarchyComponent = scene->GetRegistry().try_get<Hierarchy>(child);
				if (hierarchyComponent)
					child = hierarchyComponent->Next();
			}
		}
		return children;
	}

	bool Entity::IsParent(Entity potentialParent)
	{
		auto nodeHierarchyComponent = scene->GetRegistry().try_get<Hierarchy>(entityHandle);
		if (nodeHierarchyComponent)
		{
			auto parent = nodeHierarchyComponent->Parent();
			while (parent != entt::null)
			{
				if (parent == potentialParent.entityHandle)
				{
					return true;
				}
				else
				{
					nodeHierarchyComponent = scene->GetRegistry().try_get<Hierarchy>(parent);
					parent = nodeHierarchyComponent ? nodeHierarchyComponent->Parent() : entt::null;
				}
			}
		}
		return false;
	}

	void Entity::Destroy()
	{
		scene->GetRegistry().destroy(entityHandle);
	}

	bool Entity::Valid()
	{
		return scene->GetRegistry().valid(entityHandle) && scene;
	}

};
