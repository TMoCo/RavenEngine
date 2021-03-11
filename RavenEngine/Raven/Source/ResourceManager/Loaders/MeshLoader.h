#pragma once

#include "ILoader.h"

namespace Raven 
{
	class MeshLoader : public ILoader
	{
		MeshLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path);
	};
}
