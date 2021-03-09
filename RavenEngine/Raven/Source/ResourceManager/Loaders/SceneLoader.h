#pragma once

#include "ILoader.h"

namespace Raven
{
	class SceneLoader : ILoader
	{
	public:
		SceneLoader();

		bool LoadAsset(const std::string& path);
	};
}