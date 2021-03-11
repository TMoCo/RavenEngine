
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ImageLoader.h"

namespace Raven
{
	ImageLoader::ImageLoader(ResourceManager& initResouceManager) 
		: ILoader(initResouceManager, ELoaderType::LT_Image) {}

	bool ImageLoader::LoadAsset(const std::string& path)
	{
		// check that the resource is already in the resource register

		int width;
		int height;

		// add the resource to the resource registry

		return true;
	}
}