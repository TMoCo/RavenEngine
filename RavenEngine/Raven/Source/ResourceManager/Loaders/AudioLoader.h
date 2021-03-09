#pragma once

#include "ILoader.h"

namespace Raven
{
	class AudioLoader : ILoader
	{
	public:
		AudioLoader();

		bool LoadAsset(const std::string& path);
	};
}