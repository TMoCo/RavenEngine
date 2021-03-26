//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "SceneGraph.h"
#include "Component/Component.h"
#include "Component/Transform.h"
#include "Component/GUIComponent.h"

namespace Raven 
{
	void SceneGraph::Init(entt::registry& registry)
	{
		registry.on_construct<Hierarchy>().connect<&Hierarchy::OnConstruct>();
		registry.on_update<Hierarchy>().connect<&Hierarchy::OnUpdate>();
		registry.on_destroy<Hierarchy>().connect<&Hierarchy::OnDestroy>();
	}

	void SceneGraph::DisconnectOnConstruct(bool disable, entt::registry& registry)
	{
		if (disable)
			registry.on_construct<Hierarchy>().disconnect<&Hierarchy::OnConstruct>();
		else
			registry.on_construct<Hierarchy>().connect<&Hierarchy::OnConstruct>();
	}

	void SceneGraph::Update(entt::registry& registry)
	{
		auto nonHierarchyView = registry.view<Transform>(entt::exclude<Hierarchy>);

		for (auto entity : nonHierarchyView)
		{
			registry.get<Transform>(entity).SetWorldMatrix(glm::mat4{1.f});
		}

		auto view = registry.view<Transform, Hierarchy>();
		for (auto entity : view)
		{
			const auto hierarchy = registry.try_get<Hierarchy>(entity);
			if (hierarchy && hierarchy->Parent() == entt::null)
			{
				//Recursively update children
				UpdateTransform(entity, registry);
			}
		}

		auto mGuiView = registry.view<GUIMenu>();
		for (auto entity : mGuiView)
		{
			const auto mGUI = registry.try_get<GUIMenu>(entity);
			if (mGUI) mGUI->UpdateGUI();
		}

		auto igGuiView = registry.view<GUIInGame>();
		for (auto entity : igGuiView)
		{
			const auto mGUI = registry.try_get<GUIInGame>(entity);
			if (mGUI) mGUI->UpdateGUI();
		}
	}

	void SceneGraph::UpdateTransform(entt::entity entity, entt::registry& registry)
	{
		auto hierarchyComponent = registry.try_get<Hierarchy>(entity);
		if (hierarchyComponent)
		{
			auto transform = registry.try_get<Transform>(entity);
			if (transform)
			{
				if (hierarchyComponent->Parent() != entt::null)
				{
					auto parentTransform = registry.try_get<Transform>(hierarchyComponent->Parent());
					if (parentTransform)
					{
						transform->SetWorldMatrix(parentTransform->GetWorldMatrix());
					}
				}
				else
				{
					transform->SetWorldMatrix(glm::mat4{1.f});
				}
			}

			entt::entity child = hierarchyComponent->First();
			while (child != entt::null)
			{
				auto hierarchyComponent = registry.try_get<Hierarchy>(child);
				auto next = hierarchyComponent ? hierarchyComponent->Next() : entt::null;
				UpdateTransform(child, registry);
				child = next;
			}
		}
	}
};



