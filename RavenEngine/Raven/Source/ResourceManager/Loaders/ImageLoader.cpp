#include "ImageLoader.h"

namespace Raven
{
	ImageLoader::ImageLoader() : ILoader(ELoaderType::LT_Image) {}

	bool ImageLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}