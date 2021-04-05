//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Scene/Component/CollisionComponent.h"

namespace Raven
{
	CollisionComponent::CollisionComponent(rp3d::PhysicsWorld* initWorld) :
		world(initWorld)
	{
		// attempt to add a collider to the component on creation
		AddCollider(prim);
	}

	void CollisionComponent::SetTransform(const Transform& transform)
	{

	}

	void CollisionComponent::AddCollider(CollisionPrimitive prim)
	{

	}

}

