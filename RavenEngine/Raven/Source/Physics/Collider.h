//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <array>
#include <memory>

#include <rp3d/include/reactphysics3d/reactphysics3d.h>
#include <rp3d/include/reactphysics3d/mathematics/Transform.h>

#include <cereal/cereal.hpp>

#include "Utilities/Core.h"
#include "Utilities/ToRp3d.h"
#include "Utilities/Serialization.h"

#include "Scene/Component/Transform.h"
#include "Scene/Component/TerrainComponent.h"

//
// Component to attach to an entity that needs to be collided with another, wrapper for the rp3d collider class
//

namespace Raven
{
	class CollisionBody;
	class RigidBody;

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

	// collider shape factory, needs access to the physics common object from physics module
	namespace ColliderFactory
	{
		rp3d::CollisionShape* CreateCollisionShape(rp3d::PhysicsCommon* physicsCommon, ColliderPrimitive::Type type);
		rp3d::CollisionShape* CreateBoxShape      (rp3d::PhysicsCommon* physicsCommon, float extentX = 1.0f, float extentY = 1.0f, float extentZ = 1.0f);
		rp3d::CollisionShape* CreateSphereShape   (rp3d::PhysicsCommon* physicsCommon, float radius = 1.0f);
		rp3d::CollisionShape* CreateCapsuleShape  (rp3d::PhysicsCommon* physicsCommon, float radius = 1.0f, float height = 1.0f);
		rp3d::CollisionShape* CreateHeightShape	  (rp3d::PhysicsCommon* physicsCommon, Ptr<TerrainComponent> terrain);
	};

	// base abstract collider class
	class Collider
	{
	public:
		Collider(ColliderPrimitive::Type shapeType);
		~Collider();

		// creates the collider
		virtual void CreateCollider() = 0;

		// set the body the collider belongs to (should only be called by the body's AddCollider
		void SetBody(rp3d::CollisionBody* parentBody);

		// use a transform to place the collider relative to the body it should be attached to
		void SetTransform(const Transform& transform);

	private:
		// pointer to the collider, set by the collision body class
		rp3d::Collider* collider;

		// shape type
		ColliderPrimitive::Type type;

		// the transform relative to the origin of the body the collider will be attached to
		rp3d::Transform relativeTransform;

		// pointer to the body the collider belongs to
		rp3d::CollisionBody* body;

		// pointer to the collider shape
		rp3d::CollisionShape* shape;
	};

	class BoxCollider : public Collider
	{
	public:
		BoxCollider();
		~BoxCollider();

		// override create collider class
		void CreateCollider() override;

		void SetExtent(const rp3d::Vector3& vec);
	private:
		// inherits the same variables as the base class
		rp3d::Vector3 extent; // box extent
	};

}