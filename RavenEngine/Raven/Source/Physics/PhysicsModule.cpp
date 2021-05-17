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
	void PhysicsModule::ClearRigidBodies()
	{
		rigidBodies.clear();
	}

	void PhysicsModule::AddRigidBody()
	{
		// initialise a rigid body at the origin and place it in the vector of bodies
		rigidBodies.push_back(world->createRigidBody(rp3d::Transform::identity()));
	}

	rp3d::RigidBody* PhysicsModule::GetRigidBody(int index)
	{
		return rigidBodies[index];
	}

	void PhysicsModule::DestroyRigidBody(int index)
	{
		if (index < rigidBodies.size())
			world->destroyRigidBody(rigidBodies[index]);
	}
}