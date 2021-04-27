//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Engine.h"

#include "Physics/PhysicsModule.h"

#include "Scene/Scene.h"
#include "Scene/System/PhysicsSystem.h"
#include "Scene/Component/CollisionBody.h"
#include "Scene/Component/RigidBody.h"

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

	// called before advancing the physics simulation
	void PhysicsSystem::OnUpdate(float dt, Scene* scene)
	{
		if (Engine::Get().GetEditorState() == EditorState::Play)
		{
			// get all entities with collision bodies and tranforms
			auto& registry = scene->GetRegistry();
			auto group = scene->GetRegistry().group<RigidBody>(entt::get<Transform>);

			// loop over them all and update the collision bodies with their transforms
			/*
			*/
			char buf[128];
			sprintf(buf, "There are %i entities with rigid bodies and transforms", group.size());
			LOGV(buf);

			for (auto entity : group)
			{
				const auto& [rigBod, trans] = group.get<RigidBody, Transform>(entity);
				trans.SetTransform(Rp3dConvert::ToTransform(rp3d::Transform::interpolateTransforms(rigBod.GetPreviousState(), rigBod.GetCurrentState(), Engine::Get().GetModule<PhysicsModule>()->GetLerpFactor()))); // interpolate states to get transform used in rendering
				rigBod.SetPreviousState(rigBod.GetCurrentState()); // set previous state to current state 
				//sprintf(buf, "Entity has %i colliders", rigBod.GetNumColliders());
				//LOGV(buf);
			}
		}
	}

	void PhysicsSystem::OnImGui()
	{

	}
}