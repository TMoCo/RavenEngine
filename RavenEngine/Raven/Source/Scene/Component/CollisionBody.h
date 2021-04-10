//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "Physics/Collider.h"

#include "Scene/Component/Transform.h"

//
// Wrapper for the react collision body, also a component in the engine's ECS
//

namespace Raven
{
	class CollisionBody {
	public:
		// a collision body needs a transform to exist
		CollisionBody(Ptr<rp3d::PhysicsWorld> physWorld, Transform& transform);
		~CollisionBody();

		// add a collider to the collision body
		void AddCollider(Collider& collider);
		// remove the specified collider from the collision body
		void RemoveCollider(uint32_t index);

		// removes the body and its colliders from the physics world
		void RemoveBodyFromWorld();

		// check that the collision body exists in the physics world
		bool ExistsInWorld();

		template<class Archive>
		void save(Archive& archive) const;

		template<class Archive>
		void load(Archive& archive);

	private:
		// world the body belongs to
		Ptr<rp3d::PhysicsWorld> world;

		// keep the body as a raw pointer, managed in our deleter to destroy
		// it in the physics world
		rp3d::CollisionBody* body;

		// vector of the colliders attached to the collision body
		std::vector<std::unique_ptr<Collider>> colliders;
	};

	template<class Archive>
	void CollisionBody::save(Archive& archive) const
	{
		for (auto* pCollider : colliders)
		{
			archive(cereal::make_nvp("Colliders", ));
		}
	}

	template<class Archive>
	void CollisionBody::load(Archive& archive)
	{

	}

}