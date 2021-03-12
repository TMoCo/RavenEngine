#include "Component.h"

namespace Raven
{
	Hierarchy::Hierarchy(entt::entity p)
		:parent(p)
	{
		first = entt::null;
		next = entt::null;
		prev = entt::null;
	}
	Hierarchy::Hierarchy()
	{
		Reset();
	}
	bool Hierarchy::Compare(const entt::registry& registry, const entt::entity & rhs) const
	{
		if (rhs == entt::null || rhs == parent || rhs == prev)
		{
			return true;
		}

		if (parent == entt::null)
		{
			return false;
		}
		else
		{
			auto& thisParent = registry.get<Hierarchy>(parent);
			auto& rhsParent = registry.get<Hierarchy>(rhs).parent;
			if (thisParent.Compare(registry, parent))
			{
				return true;
			}
		}
		return false;
	}

	void Hierarchy::Reset()
	{
		parent = entt::null;
		first = entt::null;
		next = entt::null;
		prev = entt::null;
	}

	void Hierarchy::OnConstruct(entt::registry& registry, entt::entity entity)
	{
		auto& hierarchy = registry.get<Hierarchy>(entity);
		if (hierarchy.parent != entt::null) 
		{
			auto& parentHierarchy = registry.get_or_emplace<Hierarchy>(hierarchy.parent);

			if (parentHierarchy.first == entt::null)
			{
				parentHierarchy.first = entity;
			}
			else
			{
				auto prevEnt = parentHierarchy.first;
				auto currentHierarchy = registry.try_get<Hierarchy>(prevEnt);

				while (currentHierarchy != nullptr && currentHierarchy->next != entt::null)
				{
					prevEnt = currentHierarchy->next;
					currentHierarchy = registry.try_get<Hierarchy>(prevEnt);
				}
				currentHierarchy->next = entity;
				hierarchy.prev = prevEnt;
			}
		}
	}

	void Hierarchy::OnDestroy(entt::registry& registry, entt::entity entity)
	{
		auto& hierarchy = registry.get<Hierarchy>(entity);
		if (hierarchy.prev == entt::null || !registry.valid(hierarchy.prev))
		{
			if (hierarchy.parent != entt::null && registry.valid(hierarchy.parent))
			{
				auto parentHierarchy = registry.try_get<Hierarchy>(hierarchy.parent);
				if (parentHierarchy != nullptr)
				{
					parentHierarchy->first = hierarchy.next;
					if (hierarchy.next != entt::null)
					{
						auto nextHierarchy = registry.try_get<Hierarchy>(hierarchy.next);
						if (nextHierarchy != nullptr)
						{
							nextHierarchy->prev = entt::null;
						}
					}
				}
			}
		}
		else
		{
			auto prevHierarchy = registry.try_get<Hierarchy>(hierarchy.prev);
			if (prevHierarchy != nullptr)
			{
				prevHierarchy->next = hierarchy.next;
			}
			if (hierarchy.next != entt::null)
			{
				auto nextHierarchy = registry.try_get<Hierarchy>(hierarchy.next);
				if (nextHierarchy != nullptr)
				{
					nextHierarchy->prev = hierarchy.prev;
				}
			}
		}
	}

	void Hierarchy::OnUpdate(entt::registry& registry, entt::entity entity)
	{
		auto& hierarchy = registry.get<Hierarchy>(entity);
		// if is the first child
		if (hierarchy.prev == entt::null)
		{
			if (hierarchy.parent != entt::null)
			{
				auto parent_hierarchy = registry.try_get<Hierarchy>(hierarchy.parent);
				if (parent_hierarchy != nullptr)
				{
					parent_hierarchy->first = hierarchy.next;
					if (hierarchy.next != entt::null)
					{
						auto next_hierarchy = registry.try_get<Hierarchy>(hierarchy.next);
						if (next_hierarchy != nullptr)
						{
							next_hierarchy->prev = entt::null;
						}
					}
				}
			}
		}
		else
		{
			auto prev_hierarchy = registry.try_get<Hierarchy>(hierarchy.prev);
			if (prev_hierarchy != nullptr)
			{
				prev_hierarchy->next = hierarchy.next;
			}
			if (hierarchy.next != entt::null)
			{
				auto next_hierarchy = registry.try_get<Hierarchy>(hierarchy.next);
				if (next_hierarchy != nullptr)
				{
					next_hierarchy->prev = hierarchy.prev;
				}
			}
		}
	}

	void Hierarchy::Reparent(entt::entity entity, entt::entity parent, entt::registry& registry, Hierarchy& hierarchy)
	{
		Hierarchy::OnDestroy(registry, entity);

		hierarchy.parent = entt::null;
		hierarchy.next = entt::null;
		hierarchy.prev = entt::null;

		if (parent != entt::null)
		{
			hierarchy.parent = parent;
			Hierarchy::OnConstruct(registry, entity);
		}
	}

}
