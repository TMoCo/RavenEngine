//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "SystemManager.h"

namespace Raven 
{
	void SystemManager::OnUpdate(float dt, Scene* scene)
	{
		for (auto& system : systems)
			system.second->OnUpdate(dt, scene);
	}

	void SystemManager::OnImGui()
	{
		for (auto& system : systems)
			system.second->OnImGui();
	}
};

