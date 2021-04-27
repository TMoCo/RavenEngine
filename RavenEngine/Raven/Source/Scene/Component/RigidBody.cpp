//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Engine.h"

#include "Physics/PhysicsModule.h"

#include "Scene/Component/RigidBody.h"

#include "Utilities/ToRp3d.h"

#include <glm/gtc/type_ptr.hpp> // make mat from array of values

namespace Raven
{
	// default constructor to register as a valid entity
	RigidBody::RigidBody() : 
		world(Engine::Get().GetModule<PhysicsModule>()->GetCurrentWorld()),
		initTransform(rp3d::Transform::identity()),
		body(nullptr),
		mass(1.0f),
		type(RigidBodyType::Static)
	{
		InitRigidBody();
	}

	/*
	RigidBody::RigidBody(Transform& transform, RigidBodyType initType) :
		world(Engine::Get().GetModule<PhysicsModule>()->GetCurrentWorld()),
		initTransform(Rp3dConvert::ToRp3dTransform(transform)),
		body(nullptr),
		mass(1.0f),
		type(initType)
	{
		InitRigidBody();
	}
	*/

	RigidBody::~RigidBody()
	{
		colliders.clear();
		//if (body)
			//world->destroyCollisionBody(body);
	}

	void RigidBody::InitRigidBody()
	{
		previousState = initTransform;
		// if body already exists, delete it and replace
		if (body)
			world->destroyRigidBody(body);
		body = world->createRigidBody(previousState);
		body->setType(static_cast<rp3d::BodyType>(type));
	}

	void RigidBody::AddCollider(Collider* collider)
	{
		// update the parent body of the collider
		collider->SetBody(this->body);
		// adding the collider to the body makes it exist in physics world
		collider->InitShape(Engine::Get().GetModule<PhysicsModule>()->GetPhysicsCommon());
		// update the body's vector containing colliders 
		colliders.emplace_back(collider);
		LOGV(colliders.size());
	}

	void RigidBody::RemoveCollider(uint32_t index)
	{
		// NB THIS DOES NOT CHECK IF THE INDEX IS OUT OF BOUNDS (do this through editor?)
		colliders.erase(colliders.begin() + index); // deletes the pointer
	}

	std::shared_ptr<Collider> RigidBody::GetCollider(uint32_t index)
	{
		return colliders[index];
	}

	void RigidBody::SetIsTrigger(uint32_t index, bool b)
	{
		colliders[index]->collider->setIsTrigger(b);
	}

	bool RigidBody::IsTrigger(uint32_t index)
	{
		return colliders[index]->collider->getIsTrigger();
	}

	void RigidBody::SetPreviousState(const rp3d::Transform& t)
	{
		previousState = t;
	}

	rp3d::Transform RigidBody::GetPreviousState()
	{
		return previousState;
	}

	rp3d::Transform RigidBody::GetCurrentState()
	{
		return body->getTransform();
	}

	void RigidBody::SetTransform(const Transform& t)
	{
		// set the underlying rp3d body's transform to specified
		body->setTransform(Rp3dConvert::ToRp3dTransform(t));
	}

	Transform RigidBody::GetTransform()
	{
		float m[16];
		body->getTransform().getOpenGLMatrix(m); // get transform from physics library
		return Transform(glm::make_mat4(m));
	}

	void RigidBody::SetInitTransform(const Transform& t)
	{
		initTransform = Rp3dConvert::ToRp3dTransform(t);
	}

	uint32_t RigidBody::GetNumColliders()
	{
		return colliders.size();
	}

	//
	// Body settings
	//

	// set if the body is affected by gravity or not
	void RigidBody::EnableGravity(bool b)
	{
		body->enableGravity(b);
	}
	bool RigidBody::GravityEnabled()
	{
		return body->isGravityEnabled();
	}

	void RigidBody::SetMass(float m)
	{
		mass = m;
		body->setMass(mass);
	}

	float RigidBody::GetMass()
	{
		return mass;
	}

	void RigidBody::SetLinearDamping(float d)
	{
		// d >= 0 
		body->setLinearDamping(d);
	}
	void RigidBody::SetAngularDamping(float d)
	{
		// d >= 0 
		body->setAngularDamping(d);
	}

	void RigidBody::SetIsAllowedToSleep(bool b)
	{
		body->setIsAllowedToSleep(b);
	}

	bool RigidBody::CanSleep()
	{ 
		return body->isAllowedToSleep(); 
	}

	//
	// Apply forces to the rigid body (only during one call of update world in simulation)
	//

	// Applies force to the body's centre of mass
	void RigidBody::ApplyForce(const glm::vec3& f)
	{
		body->applyForceToCenterOfMass(Rp3dConvert::ToRp3dVector3(f));
	}

	void RigidBody::ApplyForceAtLocalPos(const glm::vec3& f, const glm::vec3& pos)
	{
		body->applyForceAtLocalPosition(Rp3dConvert::ToRp3dVector3(f), Rp3dConvert::ToRp3dVector3(pos));
	}

	void RigidBody::ApplyForceAtWorldPos(const glm::vec3& f, const glm::vec3& pos)
	{
		body->applyForceAtWorldPosition(Rp3dConvert::ToRp3dVector3(f), Rp3dConvert::ToRp3dVector3(pos));
	}

	void RigidBody::ApplyTorque(const glm::vec3& t)
	{
		body->applyTorque(Rp3dConvert::ToRp3dVector3(t));
	}
}