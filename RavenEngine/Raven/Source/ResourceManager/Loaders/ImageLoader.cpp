
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ImageLoader.h"


namespace Raven
{
	ImageLoader::ImageLoader(ResourceManager& initResouceManager) : ILoader(initResouceManager, ELoaderType::LT_Image) {}

	bool ImageLoader::LoadAsset(const std::string& path)
	{
		// check that the resource is already in the resource register
		if (resourceManager->HasResource(path))
		{
			return true; // already loaded, return true for now
		}

		std::cout << "Will load: " << path << '\n';
		// the test file is 500 by 500 pixels

		int width		   = 0;
		int height		   = 0;
		int channelsInFile = 0;


		// unique pointer containing data from a file, with the deleter provided by stbi
		std::unique_ptr<stbi_uc, std::function<void(void*)>> fromFile (stbi_load(path.c_str(), &width, &height, &channelsInFile, STBI_default), stbi_image_free);

		// TODO: determine image format and process accordingly
		switch (channelsInFile)
		{
		default:
			break;
		}

		// TODO: create the texture resource using a custom allocator
		// for now, do everything on the big ugly heap >:^(
		Texture2D* imageResource = new Texture2D(static_cast<size_t>(width), static_cast<size_t>(height));
		// create a new array of pixel data
		uint8_t* data = new uint8_t[width * height];
		memcpy(data, fromFile.get(), width * height * sizeof(uint8_t));
		imageResource->data = data;

		// add the resource to the resource registry
		resourceManager->AddResource(path, imageResource);

		return true;
	}
}