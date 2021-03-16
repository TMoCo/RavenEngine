#pragma once

#include "ILoader.h"

namespace Raven
{
	class MaterialLoader : ILoader
	{
	public:
		MaterialLoader(ResourceManager& initResourceManager);

		bool virtual LoadAsset(const std::string& path);
	};
}