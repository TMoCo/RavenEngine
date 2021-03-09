//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "SceneGraph.h"
#include "Component/Component.h"

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

	}

	void SceneGraph::UpdateTransform(entt::entity entity, entt::registry& registry)
	{
	}

};



