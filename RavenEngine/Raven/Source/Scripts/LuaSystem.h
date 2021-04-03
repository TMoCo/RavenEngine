//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Scene/System/ISystem.h"

namespace Raven 
{
	class Scene;
	class LuaSystem final : public ISystem
	{
	public:
		void OnInit() override;
		void OnUpdate(float dt, Scene* scene) override;
		void OnImGui() override;
	};
};