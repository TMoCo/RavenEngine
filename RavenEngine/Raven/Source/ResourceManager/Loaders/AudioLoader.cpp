//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "AudioLoader.h"

namespace Raven
{
	AudioLoader::AudioLoader(ResourceManager& initResourceManager) 
		: ILoader(initResourceManager, ELoaderType::LT_Audio) {}

	bool AudioLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}