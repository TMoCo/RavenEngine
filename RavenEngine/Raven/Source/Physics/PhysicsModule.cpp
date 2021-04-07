//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Physics/PhysicsModule.h"

namespace Raven
{
	void PhysicsModule::Initialize()
	{
		// instantiate the physics world
		world = physicsCommon.createPhysicsWorld();
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