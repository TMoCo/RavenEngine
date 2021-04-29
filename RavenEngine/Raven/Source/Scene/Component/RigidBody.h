//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <cereal/types/vector.hpp>

#include "Physics/PhysicsModule.h"
#include "Physics/Collider.h"

#include "Scene/Component/Transform.h"
#include "Scene/Component/Component.h"

//
// Wrapper component class for rp3d rigid bodies
//

namespace Raven
{
	// types of rigid body:
	// Static -> does not move and only collides with Dynamic bodies (eg: a floor)
	// Kinematic -> moves and only collides with Dynamic bodies (eg: a moving platform)
	// Dynamic -> moves and collides with all body types (eg: a player character)
	enum class RigidBodyType
	{
		Static,
		Kinematic,
		Dynamic
	};

	class PhysicsModule;

	class RigidBody : public Component
	{
		friend PhysicsModule;
	public:
		// a collision body needs a transform to exist
		RigidBody();
		//RigidBody(Transform& transform = Transform::Identity(), RigidBodyType initType = RigidBodyType::Static);
		~RigidBody();

		void InitRigidBody();
		void DestroyRigidBody();

		// add a collider to the collision body
		void AddCollider(Collider* collider);
		// remove the specified collider from the collision body
		void RemoveCollider(uint32_t index);

		// get the specified collider from the collision body colliders
		std::shared_ptr<Collider> GetCollider(uint32_t index);
		std::vector<std::shared_ptr<Collider>>* GetAllColliders();

		// set the collider at specified index to be a trigger (does not collide, only reports overlap)
		void SetIsTrigger(uint32_t index, bool b);
		bool IsTrigger(uint32_t index);

		void SetPreviousState(const rp3d::Transform& t);
		rp3d::Transform GetPreviousState();
		rp3d::Transform GetCurrentState();

		void SetInitTransform(const Transform& t);
		Transform GetInitTransform();
		void InitTransform(); // initialises the rigid body's transform
		void SetTransform(const Transform& t);
		Transform GetTransform();

		uint32_t GetNumColliders();

		// set if the body is affected by gravity or not
		void EnableGravity(bool b);
		bool GravityEnabled();

		void SetMass(float m);
		float GetMass();

		void SetLinearDamping(float d);
		void SetAngularDamping(float d);
		void SetIsAllowedToSleep(bool b);
		void SetBodyType(RigidBodyType t);

		float GetLinearDamping();
		float GetAngularDamping();
		bool CanSleep();
		RigidBodyType GetBodyType();

		// Applies forces to the body
		void ApplyForce(const glm::vec3& f);
		void ApplyForceAtLocalPos(const glm::vec3& f, const glm::vec3& pos);
		void ApplyForceAtWorldPos(const glm::vec3& f, const glm::vec3& pos);
		void ApplyTorque(const glm::vec3& t);

		// load and save the colliders attached to the body
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Type", static_cast<int>(type)),
					cereal::make_nvp("Mass", mass),
					cereal::make_nvp("Linear Damping", linearDamping),
					cereal::make_nvp("Angular Damping", angularDamping),
					cereal::make_nvp("Colliders", colliders));
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			// nb, this only loads shape data and creates instances of the collider objects.
			archive(cereal::make_nvp("Type", static_cast<RigidBodyType>(type)), 
					cereal::make_nvp("Mass", mass),
					cereal::make_nvp("Linear Damping", linearDamping),
					cereal::make_nvp("Angular Damping", angularDamping),
					cereal::make_nvp("Colliders", colliders));
			// we also need to set the body properties in the physics engine
			this->SetBodyType(type);
			this->SetMass(mass);
			this->SetLinearDamping(linearDamping);
			this->SetAngularDamping(angularDamping);
			// the colliders need to be added to the physics engine, so loop and add them to the body
			auto physCommon = Engine::Get().GetModule<PhysicsModule>()->GetPhysicsCommon();
			for (auto& collider : colliders)
			{
				// essentially the same as the AddCollider function, minus the adding to the collider vector (already there)
				collider->SetBody(this->body);
				collider->InitShape(physCommon);
				this->body->addCollider(collider->GetShape(), Rp3dConvert::ToRp3dTransform(collider->GetRelativeTransform()));
			}
		}

	private:
		// world the body belongs to
		rp3d::PhysicsWorld* world;

		// keep the body as a raw pointer, managed in our deleter to destroy
		// it in the physics world
		rp3d::RigidBody* body;

		float mass;
		float linearDamping;
		float angularDamping;

		bool canSleep;
		bool gravityEnabled;

		RigidBodyType type;

		rp3d::Transform initTransform;

		// a copy of the previous state, used for slerping 
		rp3d::Transform previousState;

		// vector of the colliders attached to the collision body
		std::vector<std::shared_ptr<Collider>> colliders;
	};


}