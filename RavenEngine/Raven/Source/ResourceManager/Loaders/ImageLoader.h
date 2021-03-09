#pragma once

#include "ILoader.h"

namespace Raven
{
	class ImageLoader : public ILoader
	{
		ImageLoader();

		bool LoadAsset(const std::string& path);
	};
}