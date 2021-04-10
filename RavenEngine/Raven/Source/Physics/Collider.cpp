//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <reactphysics3d/mathematics/Vector3.h> 

#include "Engine.h"

#include "Physics/PhysicsModule.h"
#include "Physics/Collider.h"

namespace Raven
{
	//
	// Collider base class
	//

	Collider::Collider(ColliderPrimitive::Type shapeType) :
		type(shapeType),							   // When creating a new collider, specify the type
		relativeTransform(rp3d::Transform::identity()) // initialise the relative transform to the identity matrix
	{}

	// when deleting the collider, remove it from the body it's attached to
	Collider::~Collider()
	{
		if (collider)
			body->removeCollider(collider); // takes care of deleting the data for us
	}

	// creates the collider by adding it to the body 
	void Collider::CreateCollider(rp3d::CollisionShape* shape)
	{
		std::cout << (int)shape->getType();
		collider = body->addCollider(shape, relativeTransform);
	}

	// call this when creating a collider 
	void Collider::SetBody(rp3d::CollisionBody* parentBody)
	{
		body = parentBody;
	}

	void Collider::SetTransform(const Transform& transform)
	{
		relativeTransform = ToRp3d::ToRp3dTransform(transform);
	}

	rp3d::CollisionShape* Collider::GetShape()
	{
		return shape;
	}

	rp3d::Transform Collider::GetRelativeTransform()
	{
		return relativeTransform;
	}

	//
	// Box collider class
	//

	BoxCollider::BoxCollider() : 
		Collider(ColliderPrimitive::Box)
	{}


	void BoxCollider::SetExtent(const rp3d::Vector3& vec)
	{
		extents = vec;
	}

	//
	// Sphere collider class
	//

	SphereCollider::SphereCollider() :
		Collider(ColliderPrimitive::Sphere)
	{}

	void SphereCollider::SetRadius(const float& r)
	{
		radius = r;
	}

	//
	// Capsule collider class
	//

	CapsuleCollider::CapsuleCollider() :
		Collider(ColliderPrimitive::Capsule)
	{}

	void CapsuleCollider::SetRadius(const float& r)
	{
		radius = r;
	}

	void CapsuleCollider::SetHeight(const float& h)
	{
		height = h;
	}

	//
	// Collider shape factory
	//

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

		rp3d::CollisionShape* CreateBoxShape(rp3d::PhysicsCommon* physicsCommon, rp3d::Vector3 extent)
		{
			return physicsCommon->createBoxShape(extent);
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

	//
	// Collider shape primitives
	//

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

