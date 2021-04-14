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

	ColliderPrimitive::Type Collider::GetColliderType() const
	{
		return type;
	}

	rp3d::Transform Collider::GetRelativeTransform()
	{
		return relativeTransform;
	}

	//
	// Box collider class
	//

	BoxCollider::BoxCollider() : 
		Collider(ColliderPrimitive::Box),
		extents(1.0f, 1.0f, 1.0f)
	{}

	// creates the collider by adding it to the body 
	void BoxCollider::CreateCollider(rp3d::CollisionShape* shape)
	{
		LOGV((int)shape->getType());
		extents = static_cast<rp3d::BoxShape*>(shape)->getHalfExtents();
		collider = body->addCollider(shape, relativeTransform);
	}

	void BoxCollider::SetExtents(const rp3d::Vector3& vec)
	{
		extents = vec;
	}

	rp3d::Vector3 BoxCollider::GetExtents() const
	{
		return extents;
	}

	//
	// Sphere collider class
	//

	SphereCollider::SphereCollider() :
		Collider(ColliderPrimitive::Sphere),
		radius(1.0f)
	{}

	// creates the collider by adding it to the body 
	void SphereCollider::CreateCollider(rp3d::CollisionShape* shape)
	{
		LOGV((int)shape->getType());
		radius = static_cast<rp3d::SphereShape*>(shape)->getRadius();
		collider = body->addCollider(shape, relativeTransform);
	}

	void SphereCollider::SetRadius(const float& r)
	{
		radius = r;
	}

	float SphereCollider::GetRadius() const
	{
		return radius;
	}

	//
	// Capsule collider class
	//

	CapsuleCollider::CapsuleCollider() :
		Collider(ColliderPrimitive::Capsule),
		radius(1.0f),
		height(1.0f)
	{}

	void CapsuleCollider::CreateCollider(rp3d::CollisionShape* shape)
	{
		LOGV((int)shape->getType());
		radius = static_cast<rp3d::CapsuleShape*>(shape)->getRadius();
		height = static_cast<rp3d::CapsuleShape*>(shape)->getHeight();
		collider = body->addCollider(shape, relativeTransform);
	}

	void CapsuleCollider::SetRadius(const float& r)
	{
		radius = r;
	}

	void CapsuleCollider::SetHeight(const float& h)
	{
		height = h;
	}

	float CapsuleCollider::GetRadius() const
	{
		return radius;
	}

	float CapsuleCollider::GetHeight() const
	{
		return height;
	}

	//
	// Collider shape factory
	//

	namespace ColliderShapeFactory
	{
		rp3d::CollisionShape* CreateCollisionShape(rp3d::PhysicsCommon* physicsCommon, Collider* collider)
		{
			// Use a C style cast to avoid overhead of dynamic cast, and avoid compile time error for static cast
			// Naturally, only pointers to derived colliders should be passed
			switch (collider->GetColliderType())
			{
			case Raven::ColliderPrimitive::Box:
				return CreateBoxShape(physicsCommon, (BoxCollider*) collider);
			case Raven::ColliderPrimitive::Sphere:
				return CreateSphereShape(physicsCommon, (SphereCollider*) collider);
			case Raven::ColliderPrimitive::Capsule:
				return CreateCapsuleShape(physicsCommon, (CapsuleCollider*) collider);
			default:
				return nullptr;
				// height must be handled differently so not in this method
			}
		}

		rp3d::CollisionShape* CreateBoxShape(rp3d::PhysicsCommon* physicsCommon, BoxCollider* collider)
		{
			return physicsCommon->createBoxShape(collider->GetExtents());
		}

		rp3d::CollisionShape* CreateSphereShape(rp3d::PhysicsCommon* physicsCommon, SphereCollider* collider)
		{
			return physicsCommon->createSphereShape(collider->GetRadius());
		}

		rp3d::CollisionShape* CreateCapsuleShape(rp3d::PhysicsCommon* physicsCommon, CapsuleCollider* collider)
		{
			return physicsCommon->createCapsuleShape(collider->GetRadius(), collider->GetHeight());
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

