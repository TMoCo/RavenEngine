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
}