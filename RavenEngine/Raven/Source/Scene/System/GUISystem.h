#pragma once
#include "ISystem.h"

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