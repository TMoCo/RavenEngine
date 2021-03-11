#pragma once

#include "ILoader.h"

namespace Raven
{
	class MaterialLoader : ILoader
	{
	public:
		MaterialLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path);
	};
}