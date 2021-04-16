//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Scene/Scene.h"
#include "Scene/System/PhysicsSystem.h"
#include "Scene/Component/CollisionBody.h"

//
// System for managing all physics components
//

namespace Raven
{
	PhysicsSystem::PhysicsSystem()
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::OnInit()
	{

	}

	void PhysicsSystem::OnUpdate(float dt, Scene* scene)
	{
		// get all entities with collision bodies and tranforms
		auto& registry = scene->GetRegistry();
		auto bodiesWithTransform = registry.view<Transform, CollisionBody>();

		// loop over them all and update the collision bodies with their transforms
		for (auto entity : bodiesWithTransform)
		{
			registry.get<CollisionBody>(entity).SetTransform(registry.get<Transform>(entity));
		}
	}

	void PhysicsSystem::OnImGui()
	{

	}
}