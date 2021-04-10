//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <array>
#include <memory>

#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/mathematics/Transform.h>

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
	namespace ColliderShapeFactory
	{
		rp3d::CollisionShape* CreateCollisionShape(rp3d::PhysicsCommon* physicsCommon, ColliderPrimitive::Type type);
		rp3d::CollisionShape* CreateBoxShape      (rp3d::PhysicsCommon* physicsCommon, rp3d::Vector3 extent = rp3d::Vector3(1.0f, 1.0f, 1.0f));
		rp3d::CollisionShape* CreateSphereShape   (rp3d::PhysicsCommon* physicsCommon, float radius = 1.0f);
		rp3d::CollisionShape* CreateCapsuleShape  (rp3d::PhysicsCommon* physicsCommon, float radius = 1.0f, float height = 1.0f);
		rp3d::CollisionShape* CreateHeightShape	  (rp3d::PhysicsCommon* physicsCommon, Ptr<TerrainComponent> terrain);
	};

	// base collider class
	class Collider
	{
		friend CollisionBody;
		friend RigidBody;
	public:
		Collider(ColliderPrimitive::Type shapeType);
		~Collider();

		// creates the collider and adds it to the body 
		void CreateCollider(rp3d::CollisionShape* shape);

		// set the body the collider belongs to (should only be called by the body's AddCollider prior to calling CreateCollider)
		void SetBody(rp3d::CollisionBody* parentBody);

		// use a transform to place the collider relative to the body it should be attached to
		void SetTransform(const Transform& transform);

		rp3d::CollisionShape* GetShape();

		rp3d::Transform GetRelativeTransform();

	protected:
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
		~BoxCollider() = default;

		void SetExtent(const rp3d::Vector3& vec);

		template<class Archive>
		void save(Archive& archive);

		template<class Archive>
		void load(Archive& archive);

	private:
		// inherits the same variables as the base class
		rp3d::Vector3 extents; // box extent
	};

	template<class Archive>
	void BoxCollider::save(Archive& archive)
	{
		// get the extents from the shape
		extents = static_cast<rp3d::BoxShape*>(shape)->getHaflExtents();
		// then archive the collider
		archive(cereal::make_nvp("Half extents", extents));
	}

	template<class Archive>
	void BoxCollider::load(Archive& archive)
	{
		// load the extents
		archive(cereal::make_nvp("Half extents", extents));
	}

	class SphereCollider : public Collider
	{
	public:
		SphereCollider();
		~SphereCollider() = default;

		void SetRadius(const float& r);

		template<class Archive>
		void save(Archive& archive);

		template<class Archive>
		void load(Archive& archive);

	private:
		float radius; // sphere radius
	};

	template<class Archive>
	void SphereCollider::save(Archive& archive)
	{
		// get the extents from the shape
		radius = static_cast<rp3d::SphereShape*>(shape)->getRadius();
		// then archive the collider
		archive(cereal::make_nvp("Radius", radius));
	}

	template<class Archive>
	void SphereCollider::load(Archive& archive)
	{
		archive(cereal::make_nvp("Radius", radius));
	}

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider();
		~CapsuleCollider() = default;

		void SetRadius(const float& r);
		void SetHeight(const float& h);

		template<class Archive>
		void save(Archive& archive);

		template<class Archive>
		void load(Archive& archive);

	private:
		float radius; // capsule radius
		float height; // capsule height
	};

	template<class Archive>
	void CapsuleCollider::save(Archive& archive)
	{
		// get the extents from the shape
		radius = static_cast<rp3d::CapsuleShape*>(shape)->getRadius();
		// then archive the collider
		archive(cereal::make_nvp("Radius", radius), cereal::make_nvp("Height", height));
	}

	template<class Archive>
	void CapsuleCollider::load(Archive& archive)
	{
		archive(cereal::make_nvp("Radius", radius), cereal::make_nvp("Height", height));
	}
}