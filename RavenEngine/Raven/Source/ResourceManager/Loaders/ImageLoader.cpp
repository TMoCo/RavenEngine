//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Resources/Texture2D.h"


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

		int width		   = 0;
		int height		   = 0;
		int channelsInFile = 0;

		// unique pointer containing data from a file, with the deleter provided by stbi
		std::unique_ptr<stbi_uc, std::function<void(void*)>> fromFile (stbi_load(path.c_str(), &width, &height, &channelsInFile, STBI_default), stbi_image_free);

		// no data...
		if (!fromFile.get())
		{
			return false;
		}

		EGLFormat format;
		switch (channelsInFile)
		{
		case STBI_grey:
			format = EGLFormat::R;
			break;
		case STBI_rgb:
			format = EGLFormat::RGB;
			break;
		case STBI_rgb_alpha:
			format = EGLFormat::RGBA;
			break;
		default:
			format = EGLFormat::None;
			break;
		}

		// create a new array of pixel data, Texture2D takes care of deleting the data
		byte* data = new byte[static_cast<size_t>(width * height)];
		memcpy(data, fromFile.get(), static_cast<size_t>(width * height) * sizeof(byte));

		// TODO: create the texture resource using a custom allocator
		// for now, do everything on the big ugly heap >:^(
		Texture2D* imageResource = new Texture2D(static_cast<size_t>(width), static_cast<size_t>(height), format, data);

		// add the resource to the resource registry
		resourceManager->AddResource(path, imageResource);

		return true; // smart pointer frees image data out of scope
	}
}