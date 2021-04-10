//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "IModule.h"

#include "Utilities/Core.h"

namespace Raven
{
	// ideal time step of 1 / 60 s
	constexpr float timeStep = 1.0f / 60.0f;

	class PhysicsModule : public IModule
	{
	public:
		PhysicsModule() = default;
		virtual ~PhysicsModule() override {};

		// inherited from IModule, must be overridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		static EModuleType GetModuleType() { return EModuleType::MT_Physics; }
		
		// update the physics world
		void Step(float deltaTime);

	private:
		// singleton class for physics library's memory allocation etc
		rp3d::PhysicsCommon physicsCommon;
		// physics world where collisions and simulations are run
		rp3d::PhysicsWorld* world;

		// where we accumulate time between frames
		float accumulator;
	};
}
