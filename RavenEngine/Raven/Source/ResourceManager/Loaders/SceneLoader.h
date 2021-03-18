//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ILoader.h"

//
// Loader for loading a scene, should take care of calling the appropriate loaders for resources used in it
//

namespace Raven
{
	class SceneLoader : ILoader
	{
	public:
		SceneLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path);
	};
}