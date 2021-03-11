#pragma once

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/ResourceManager.h"

namespace Raven
{
	class ImageLoader : public ILoader
	{
		ImageLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path);
	};
}