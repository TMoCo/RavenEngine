//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <cereal/types/vector.hpp>

#include "Physics/Collider.h"

#include "Scene/Component/Transform.h"

//
// Wrapper component class for rp3d rigid bodies
//

namespace Raven
{

	enum class RigidBodyType
	{
		Static,
		Kinematic,
		Dynamic
	};

	class PhysicsModule;

	class RigidBody
	{
		friend PhysicsModule;
	public:
		// a collision body needs a transform to exist
		RigidBody(rp3d::PhysicsWorld* physWorld, Transform& transform = Transform::Identity(), RigidBodyType initType = RigidBodyType::Static);
		~RigidBody();

		// add a collider to the collision body
		void AddCollider(Collider* collider);
		// remove the specified collider from the collision body
		void RemoveCollider(uint32_t index);

		// get the specified collider from the collision body colliders
		std::shared_ptr<Collider> GetCollider(uint32_t index);

		// set the collider at specified index to be a trigger (does not collide, only reports overlap)
		void SetIsTrigger(uint32_t index, bool b);
		bool IsTrigger(uint32_t index);

		void SetPreviousState(const rp3d::Transform& t);
		rp3d::Transform GetPreviousState();
		rp3d::Transform GetCurrentState();

		void SetTransform(const Transform& t);
		Transform GetTransform();

		uint32_t GetNumColliders();

		// set if the body is affected by gravity or not
		void EnableGravity(bool b);

		void SetLinearDamping(float d);
		void SetAngularDamping(float d);
		void SetIsAllowedToSleep(bool b);

		// Applies forces to the body
		void ApplyForce(const glm::vec3& f);
		void ApplyForceAtLocalPos(const glm::vec3& f, const glm::vec3& pos);
		void ApplyForceAtWorldPos(const glm::vec3& f, const glm::vec3& pos);
		void ApplyTorque(const glm::vec3& t);

		// load and save the colliders attached to the body
		template<class Archive>
		void save(Archive& archive) const;

		template<class Archive>
		void load(Archive& archive);

	private:
		// world the body belongs to
		rp3d::PhysicsWorld* world;

		// keep the body as a raw pointer, managed in our deleter to destroy
		// it in the physics world
		rp3d::RigidBody* body;

		float mass;

		RigidBodyType type;

		// a copy of the previous state, used for slerping 
		rp3d::Transform previousState;

		// vector of the colliders attached to the collision body
		std::vector<std::shared_ptr<Collider>> colliders;
	};

	template<class Archive>
	void RigidBody::save(Archive& archive) const
	{
		archive(cereal::make_nvp("Type", static_cast<int>(type)), cereal::make_nvp("Colliders", colliders));
	}

	template<class Archive>
	void RigidBody::load(Archive& archive)
	{
		// nb, this only loads shape data and creates instances of the collider objects.
		archive(cereal::make_nvp("Type", static_cast<RigidBodyType>(type)), cereal::make_nvp("Colliders", colliders));
	}
}