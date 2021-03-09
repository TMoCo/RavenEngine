#pragma once

#include "ILoader.h"

namespace Raven
{
	class MaterialLoader : ILoader
	{
	public:
		MaterialLoader();

		bool LoadAsset(const std::string& path);
	};
}