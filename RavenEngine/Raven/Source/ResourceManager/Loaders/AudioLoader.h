#pragma once

#include "ILoader.h"

namespace Raven
{
	class AudioLoader : ILoader
	{
	public:
		AudioLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path);
	};
}