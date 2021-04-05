//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <rp3d/include/reactphysics3d/reactphysics3d.h> 

#include "Scene/Component/Transform.h"

//
// Component to attach to an entity that needs to be collided with another
//

namespace Raven
{
	enum class CollisionPrimitive : unsigned char
	{
		Box,
		Sphere,
		Capsule,
		Height,
		None
	};

	class CollisionComponent
	{
	public:
		CollisionComponent(rp3d::PhysicsWorld* initWorld);
		~CollisionComponent();

		// use a transform to change the collider
		void SetTransform(const Transform& transform);
		// add a collision shape to the component, with an optional relative transform
		void AddCollider(CollisionPrimitive type, Transform& transform = Transform::Identity());
	private:
		rp3d::PhysicsWorld* world;

		CollisionPrimitive type;

		rp3d::CollisionShape* shape;
	};
}