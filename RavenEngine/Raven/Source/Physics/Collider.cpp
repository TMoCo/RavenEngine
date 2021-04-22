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

	Collider::Collider(rp3d::CollisionBody* initBody, ColliderPrimitive::Type shapeType, rp3d::Transform& t) :
		type(shapeType),							   // When creating a new collider, specify the type
		body(initBody),
		collider(nullptr),
		relativeTransform(t) // initialise the relative transform to the identity matrix
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
		relativeTransform = Rp3dConvert::ToRp3dTransform(transform);
	}

	ColliderPrimitive::Type Collider::GetColliderType() const
	{
		return type;
	}

	rp3d::CollisionShape* Collider::GetShape()
	{
		if (collider)
			return collider->getCollisionShape();
	}

	rp3d::Transform Collider::GetRelativeTransform()
	{
		return relativeTransform;
	}

	//
	// Box collider class
	//

	BoxCollider::BoxCollider() :
		Collider(nullptr, ColliderPrimitive::Box),
		extents(1.0f, 1.0f, 1.0f)
	{
		// don't init shape here since no body provided
	}

	BoxCollider::BoxCollider(rp3d::CollisionBody* body, Transform& t) :
		Collider(body, ColliderPrimitive::Box, Rp3dConvert::ToRp3dTransform(t)),
		extents(1.0f, 1.0f, 1.0f)
	{
		//InitShape(Engine::Get().GetModule<PhysicsModule>()->GetPhysicsCommon());
	}

	// creates the collider by adding it to the body 
	void BoxCollider::InitShape(rp3d::PhysicsCommon* physicsCommon)
	{
		// only init shape when body is present and collider is nullptr
		if (body && collider == nullptr)
		{
			rp3d::CollisionShape* s = ColliderShapeFactory::CreateBoxShape(physicsCommon, this);
			collider = body->addCollider(s, relativeTransform);
		}
	}

	void BoxCollider::SetExtents(const rp3d::Vector3& vec)
	{
		// update the extents
		extents = vec;
		// if collider was assigned a body, it exists in physics engine so also update extents there
		if (body)
			((rp3d::BoxShape*)collider->getCollisionShape())->setHalfExtents(vec);
	}

	void BoxCollider::SetExtents(const float& x, const float& y, const float& z)
	{
		extents = rp3d::Vector3(x,y,z);
		if (body)
			((rp3d::BoxShape*)collider->getCollisionShape())->setHalfExtents(extents);
	}

	rp3d::Vector3 BoxCollider::GetExtents() const
	{
		return extents;
	}

	//
	// Sphere collider class
	//
	
	SphereCollider::SphereCollider() :
		Collider(nullptr, ColliderPrimitive::Sphere),
		radius(1.0f)
	{}

	SphereCollider::SphereCollider(rp3d::CollisionBody* body) :
		Collider(body, ColliderPrimitive::Sphere),
		radius(1.0f)
	{
		InitShape(Engine::Get().GetModule<PhysicsModule>()->GetPhysicsCommon());
	}

	// creates the collider by adding it to the body 
	void SphereCollider::InitShape(rp3d::PhysicsCommon* physicsCommon)
	{
		if (body)
			collider = body->addCollider(ColliderShapeFactory::CreateSphereShape(physicsCommon, this), relativeTransform);
	}

	void SphereCollider::SetRadius(const float& r)
	{
		radius = r;
		// if collider was assigned a body, it exits in physics engine so also update extents there
		if (body)
			((rp3d::SphereShape*)collider->getCollisionShape())->setRadius(r);
	}

	float SphereCollider::GetRadius() const
	{
		return radius;
	}

	//
	// Capsule collider class
	//

	CapsuleCollider::CapsuleCollider() :
		Collider(nullptr, ColliderPrimitive::Capsule),
		radius(1.0f),
		height(1.0f)
	{}

	CapsuleCollider::CapsuleCollider(rp3d::CollisionBody* body) :
		Collider(body, ColliderPrimitive::Capsule),
		radius(1.0f),
		height(1.0f)
	{
		InitShape(Engine::Get().GetModule<PhysicsModule>()->GetPhysicsCommon());
	}

	void CapsuleCollider::InitShape(rp3d::PhysicsCommon* physicsCommon)
	{
		if (body)
			collider = body->addCollider(ColliderShapeFactory::CreateCapsuleShape(physicsCommon, this), relativeTransform);
	}

	void CapsuleCollider::SetRadius(const float& r)
	{
		radius = r;
		if (body)
			((rp3d::SphereShape*)collider->getCollisionShape())->setRadius(r);
	}

	void CapsuleCollider::SetHeight(const float& h)
	{
		height = h;
		if (body)
			((rp3d::CapsuleShape*)collider->getCollisionShape())->setHeight(h);
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

