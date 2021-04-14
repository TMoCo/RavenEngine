 //////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include <rp3d/include/reactphysics3d/reactphysics3d.h> 

#include "Scene/Component/CollisionBody.h"

#include "Utilities/ToRp3d.h"

namespace Raven
{
	CollisionBody::CollisionBody(rp3d::PhysicsWorld* physWorld, Transform& transform) :
		world(physWorld),
		body(nullptr) // initialise the body pointer to nullptr
	{
		// convert our transform component to the transform used in react and put the collision body in the 
		// physics world
		body = physWorld->createCollisionBody(ToRp3d::ToRp3dTransform(transform));
	}

	CollisionBody::~CollisionBody()
	{
		// clear the collider shapes first (collider destructors require valid collision body)
		colliders.clear();
		// make sure the body is removed from the world on desruction
		RemoveBodyFromWorld();
	}

	// add a collider to the collision body
	void CollisionBody::AddCollider(Collider* collider)
	{
		if (ExistsInWorld())
		{
			// add the collider to the body, returns pointer to the collider which we set in the wrapper Collider class
			//collider->collider = body->addCollider(collider->shape, collider->relativeTransform);
			//collider->SetBody(this->body);
			// add it to the vector of colliders
			colliders.emplace_back(collider);
		}
	}

	void CollisionBody::RemoveCollider(uint32_t index)
	{
		// NB THIS DOES NOT CHECK IF THE INDEX IS OUT OF BOUNDS (do this through editor?)
		colliders.erase(colliders.begin() + index); // deletes the pointer
	}

	void CollisionBody::RemoveBodyFromWorld()
	{
		// check the body exists
		if (ExistsInWorld())
		{
			world->destroyCollisionBody(body); // takes care of destroying the shapes associated with the body
			body = nullptr; // set to nullptr for future reference
		}
	}

	bool CollisionBody::ExistsInWorld()
	{
		if (body)
			return true;
		return false;
	}
}