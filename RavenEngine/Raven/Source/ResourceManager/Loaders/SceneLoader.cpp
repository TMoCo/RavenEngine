//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "SceneLoader.h"

namespace Raven
{
	SceneLoader::SceneLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_Audio) {}

	bool SceneLoader::LoadAsset(const std::string& path) 
	{
		// should take advantage of the cereal library here
		return true;
	}
}