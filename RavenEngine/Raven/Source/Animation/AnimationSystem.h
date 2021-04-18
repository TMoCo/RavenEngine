//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Scene/System/ISystem.h"

namespace Raven
{
	class Animation;
	class AnimationSystem : public ISystem 
	{
	public:
		AnimationSystem();
		virtual void OnInit() override;
		virtual void OnUpdate(float dt, Scene* scene) override;
		virtual void OnImGui() override;
	};
};