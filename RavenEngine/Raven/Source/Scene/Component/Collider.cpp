//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include <rp3d/include/reactphysics3d/mathematics/Vector3.h> 

#include "Scene/Component/Collider.h"

namespace Raven
{
	Collider::Collider(rp3d::PhysicsWorld* initWorld)
	{
		world = std::make_shared<rp3d::PhysicsWorld>(initWorld);
		// attempt to add a collider to the component on creation
	}

	void Collider::SetTransform(const Transform& transform)
	{

	}

	void Collider::SetShapeType(ColliderPrimitive::Type shapeType)
	{
		type = shapeType;
	}

	namespace ColliderShapeFactory
	{
		rp3d::CollisionShape* CreateCollisionShape(rp3d::PhysicsCommon* physicsCommon, ColliderPrimitive::Type type)
		{
			switch (type)
			{
			case Raven::ColliderPrimitive::Box:
				return CreateBoxShape(physicsCommon);
			case Raven::ColliderPrimitive::Sphere:
				return CreateSphereShape(physicsCommon);
			case Raven::ColliderPrimitive::Capsule:
				return CreateCapsuleShape(physicsCommon);
			default:
				return nullptr;
				// height must be handled differently so not in this method
			}
		}

		rp3d::CollisionShape* CreateBoxShape(rp3d::PhysicsCommon* physicsCommon, float extentX, float extentY, float extentZ) 
		{
			return physicsCommon->createBoxShape(rp3d::Vector3(extentX, extentY, extentZ));
		}

		rp3d::CollisionShape* CreateSphereShape(rp3d::PhysicsCommon* physicsCommon, float radius)
		{
			return physicsCommon->createSphereShape(radius);
		}

		rp3d::CollisionShape* CreateCapsuleShape(rp3d::PhysicsCommon* physicsCommon, float radius, float height)
		{
			return physicsCommon->createCapsuleShape(radius, height);
		}

		rp3d::CollisionShape* CreateHeightShape(rp3d::PhysicsCommon* physicsCommon, Ptr<TerrainComponent> terrain)
		{
			// TODO: CHANGE MIN AND MAX HEIGHT IN HEIGHT COLLIDER TO REFLECT ACTUAL VALUES
			// TODO: PROBLEM WITH HEIGHT MAP IN PHYSICS ENGINE MUST BE AT LEAST 32 BYTES
			auto* heightMap = terrain->GetTerrainResource()->heightMap;
			return physicsCommon->createHeightFieldShape(
				heightMap->width,		// columns
				heightMap->height,		// rows
				0.0f,					// min height
				100.0f,					// max height
				heightMap->data,		// ptr to height data
				rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE
			);
		}
	}

	namespace ColliderPrimitive
	{
		std::string GetColliderPrimitiveName(Type type)
		{
			return NAMES[(uint32_t)type];
		};

		Type GetColliderPrimitiveName(const std::string& type)
		{
			if (type == "Box")
			{
				return Type::Box;
			}
			else if (type == "Sphere")
			{
				return Type::Sphere;
			}
			else if (type == "Capsule")
			{
				return Type::Capsule;
			}
			else if (type == "Height")
			{
				return Type::Height;
			}
			return Type::Box;
		}
	}
}

