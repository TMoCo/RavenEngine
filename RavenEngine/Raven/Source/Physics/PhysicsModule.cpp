//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Physics/PhysicsModule.h"
#include "Physics/Collider.h"

#include "Scene/Component/CollisionBody.h"
#include "Scene/Component/RigidBody.h"

namespace Raven
{
	PhysicsModule::PhysicsModule() :
		accumulator(0.0f),
		lerpFactor(0.0f),
		world(nullptr)
	{

	}

	void PhysicsModule::Initialize()
	{
		// instantiate the physics world
		world = physicsCommon.createPhysicsWorld();
	}

	void PhysicsModule::Destroy()
	{
		// physics common is destroyed here, takes care of destroying everything in physics engine
	}

	void PhysicsModule::Step(float deltaTime)
	{
		// Add the time difference in the accumulator 
		accumulator += deltaTime;

		// While there is enough accumulated time to take one or several physics steps 
		while (accumulator >= timeStep) {

			// Update the Dynamics world with a constant time step, updates the transforms in the bodies
			world->update(timeStep);

			// Decrease the accumulated time 
			accumulator -= timeStep;
		}

		// time left in accumulator is smaller than delta time, so we normalise to get a value between 0 and 1 for lerping
		lerpFactor = accumulator / timeStep;
	}

	float PhysicsModule::GetLerpFactor()
	{
		return lerpFactor;
	}

	rp3d::PhysicsCommon* PhysicsModule::GetPhysicsCommon()
	{
		return &physicsCommon;
	}

	rp3d::PhysicsWorld* PhysicsModule::GetCurrentWorld()
	{
		return world;
	}

	// call these methods when loading in a new scene to initialise the physics world
	void PhysicsModule::CreateWorld()
	{
		// valid world pointer means we do not have to create one
		if (world)
			return;
		world = physicsCommon.createPhysicsWorld();
	}

	void PhysicsModule::DestroyWorld() // call when destroying a scene
	{
		if (world)
			physicsCommon.destroyPhysicsWorld(world);
		world = nullptr; // invalidate pointer
	}

	void PhysicsModule::RecreateWorld()
	{
		DestroyWorld();
		CreateWorld();
	}
}