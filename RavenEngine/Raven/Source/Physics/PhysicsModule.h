//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <rp3d/include/reactphysics3d/reactphysics3d.h> 

#include "IModule.h"

namespace Raven
{
	class PhysicsModule : public IModule
	{
	public:
		PhysicsModule() = default;
		virtual ~PhysicsModule() override {};

		// inherited from IModule, must be overridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		static EModuleType GetModuleType() { return EModuleType::MT_Physics; }

	private:
		// singleton class for physics library memory allocation etc
		rp3d::PhysicsCommon physicsCommon;
		// physics world where collisions and simulations are run
		rp3d::PhyscisWorld* world; 
	};
}
