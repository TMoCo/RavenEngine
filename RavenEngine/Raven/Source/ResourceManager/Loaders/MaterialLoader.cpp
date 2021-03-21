//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "MaterialLoader.h"

namespace Raven
{
	MaterialLoader::MaterialLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_Material) {}

	bool MaterialLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}