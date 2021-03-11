#pragma once

#include "ILoader.h"

namespace Raven
{
	class SceneLoader : ILoader
	{
	public:
		SceneLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path);
	};
}