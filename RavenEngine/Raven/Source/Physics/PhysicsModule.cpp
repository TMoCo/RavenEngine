//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Physics/PhysicsModule.h"
#include "Physics/Collider.h"

#include "Scene/Component/CollisionBody.h"

namespace Raven
{
	void PhysicsModule::Initialize()
	{
		// instantiate the physics world
		world = physicsCommon.createPhysicsWorld();

		// create a collision body at the origin
		CollisionBody* body = new CollisionBody(world, Transform::Identity());

		// declare a box collider 
		Collider* collider = new Collider(ColliderPrimitive::Type::Box);
		// set the body the collider should belong to
		collider->SetBody(body->body);
		// create the collider shape with physics common
		collider->CreateCollider(ColliderShapeFactory::CreateBoxShape(&physicsCommon));

		// assign the collider to the body
		body->AddCollider(collider);
	}

	void PhysicsModule::Destroy()
	{
		
	}

	void PhysicsModule::Step(float deltaTime)
	{
		// Add the time difference in the accumulator 
		accumulator += deltaTime;

		// While there is enough accumulated time to take one or several physics steps 
		while (accumulator >= timeStep) {

			// Update the Dynamics world with a constant time step 
			world->update(timeStep);

			// Decrease the accumulated time 
			accumulator -= timeStep;
		}

		// slerp between world states using the remaining accumulated time
	}
}