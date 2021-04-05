//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ISystem.h"

//
// System for managing all physics components
//

namespace Raven
{
	class GUISystem : public ISystem
	{
	public:
		GUISystem();
		~GUISystem() override;
		virtual void OnInit() override;

		virtual void OnUpdate(float dt, Scene* scene)override;

		virtual void OnImGui()override;
	};
}