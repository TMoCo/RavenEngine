//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <array>
#include <memory>

#include <rp3d/include/reactphysics3d/reactphysics3d.h> 

#include "Utilities/Core.h"

#include "Scene/Component/Transform.h"
#include "Scene/Component/TerrainComponent.h"

//
// Component to attach to an entity that needs to be collided with another, wrapper for the rp3d collider class
//

namespace Raven
{
	namespace ColliderPrimitive
	{
		enum Type : uint32_t
		{
			Box,
			Sphere,
			Capsule,
			Height,
			MAX
		};

		const std::array<std::string, Type::MAX> NAMES =
		{
			"Box",
			"Sphere",
			"Capsule",
			"Height"
		};

		std::string GetColliderPrimitiveName(ColliderPrimitive::Type type);
		ColliderPrimitive::Type GetColliderPrimitiveName(const std::string& type);
	}

	// collider shape factory class, needs access to the physics common object from physics module
	namespace ColliderShapeFactory
	{
	
		rp3d::CollisionShape* CreateCollisionShape(rp3d::PhysicsCommon* physicsCommon, ColliderPrimitive::Type type);
		rp3d::CollisionShape* CreateBoxShape      (rp3d::PhysicsCommon* physicsCommon, float extentX = 1.0f, float extentY = 1.0f, float extentZ = 1.0f);
		rp3d::CollisionShape* CreateSphereShape   (rp3d::PhysicsCommon* physicsCommon, float radius = 1.0f);
		rp3d::CollisionShape* CreateCapsuleShape  (rp3d::PhysicsCommon* physicsCommon, float radius = 1.0f, float height = 1.0f);
		rp3d::CollisionShape* CreateHeightShape   (rp3d::PhysicsCommon * physicsCommon, Ptr<TerrainComponent> terrain);
	};

	class Collider
	{
	public:
		Collider(rp3d::PhysicsWorld* initWorld);
		~Collider();

		// use a transform to change the collider
		void SetTransform(const Transform& transform);

		// set the collision shape, with an optional relative transform. Destroys the already existing collider and its shape
		void SetShapeType(ColliderPrimitive::Type shapeType);



	private:
		// pointer to the physics world
		Ptr<rp3d::PhysicsWorld> world;

		// pointer to the collider shape, if we want to edit the shape's properties like height, width, &c in the editor
		Ptr<rp3d::CollisionShape> shape;
		ColliderPrimitive::Type type;

		// the react collider
		Ptr<rp3d::Collider> collider;
	};
}