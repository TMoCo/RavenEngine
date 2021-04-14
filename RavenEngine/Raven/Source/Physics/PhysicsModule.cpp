//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Physics/PhysicsModule.h"
#include "Physics/Collider.h"

#include "Scene/Component/CollisionBody.h"

namespace Raven
{
	void PhysicsModule::Initialize()
	{
		// instantiate the physics world
		world = physicsCommon.createPhysicsWorld();

		// create a collision body at the origin
		CollisionBody* bodyA = new CollisionBody(world, Transform::Identity());

		// declare a box collider 
		BoxCollider* colliderA = new BoxCollider();
		// set the body the collider should belong to
		colliderA->SetBody(bodyA->body);
		// set our extents
		colliderA->SetExtents(rp3d::Vector3(10.0f, 20.0f, 5.0f));
		// create the collider shape with physics common
		colliderA->CreateCollider(ColliderShapeFactory::CreateBoxShape(&physicsCommon, colliderA));

		// assign the collider to the body
		bodyA->AddCollider(colliderA);

		// declare a box collider 
		SphereCollider* colliderB = new SphereCollider();
		// set the body the collider should belong to
		colliderB->SetBody(bodyA->body);
		// set a radius
		colliderB->SetRadius(15.0f);
		// create the collider shape with physics common
		colliderB->CreateCollider(ColliderShapeFactory::CreateSphereShape(&physicsCommon, colliderB));

		// assign the collider to the body
		bodyA->AddCollider(colliderB);

		// declare a box collider 
		CapsuleCollider* colliderC = new CapsuleCollider();
		// set the body the collider should belong to
		colliderC->SetBody(bodyA->body);
		// create the collider shape with physics common
		colliderC->CreateCollider(ColliderShapeFactory::CreateCapsuleShape(&physicsCommon, colliderC));

		// assign the collider to the body
		bodyA->AddCollider(colliderC);

		{
			std::ofstream os("firstSave.json");
			cereal::JSONOutputArchive oArchive(os);
			oArchive(*bodyA);
		}

		// remove the body 
		delete bodyA;

		// create a collision body above the origin
		Transform t;
		t.SetLocalPosition(glm::vec3(0.0f, 2.0f, 0.0f));
		CollisionBody* bodyB = new CollisionBody(world, t);

		{
			std::ifstream is("firstSave.json");
			cereal::JSONInputArchive iArchive(is);
			iArchive(*bodyB);
			// we have loaded collider data, but they neeed to be initialised in the rp3d library memory
			for (auto& c : bodyB->colliders)
			{
				c->SetBody(bodyB->body);
				c->CreateCollider(ColliderShapeFactory::CreateCollisionShape(&physicsCommon, c.get()));
			}
		}

		LOGE(bodyB->colliders.size());

		{
			std::ofstream os("secondSave.json");
			cereal::JSONOutputArchive oArchive(os);
			oArchive(*bodyB);
		}
	}

	void PhysicsModule::Destroy()
	{
		
	}

	void PhysicsModule::Step(float deltaTime)
	{
		// Add the time difference in the accumulator 
		accumulator += deltaTime;

		// While there is enough accumulated time to take one or several physics steps 
		while (accumulator >= timeStep) {

			// Update the Dynamics world with a constant time step 
			world->update(timeStep);

			// Decrease the accumulated time 
			accumulator -= timeStep;
		}

		// slerp between world states using the remaining accumulated time
	}
}