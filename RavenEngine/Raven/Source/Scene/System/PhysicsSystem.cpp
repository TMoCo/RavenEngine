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
			// Update physics world, generates new transforms for entities with rigid bodies
			Engine::Get().GetModule<PhysicsModule>()->Step(dt);

			//LOGE(Engine::Get().GetModule<PhysicsModule>()->GetCurrentWorld()->getNbRigidBodies());

			// get all entities with collision bodies and tranforms
			/*
			*/
			auto& registry = scene->GetRegistry();
			auto& group = scene->GetRegistry().group<RigidBody, Transform>();
			for (auto entity : group)
			{
				const auto& [rigBod, trans] = group.get<RigidBody, Transform>(entity);
				auto scale = rigBod.initTransform.GetScale();


				trans.SetTransform(Rp3dConvert::ToTransform(rp3d::Transform::interpolateTransforms(rigBod.GetPreviousState(), rigBod.GetCurrentState(), Engine::Get().GetModule<PhysicsModule>()->GetLerpFactor()), scale)); // interpolate states to get transform used in rendering
				rigBod.SetPreviousState(rigBod.GetCurrentState()); // set previous state to current state 
			}
		}
	}

	void PhysicsSystem::OnImGui()
	{

	}
}