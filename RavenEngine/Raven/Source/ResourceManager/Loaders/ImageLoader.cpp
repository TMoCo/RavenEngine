#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Resources/Texture2D.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#include <iostream>
#include <fstream>




namespace Raven
{


ImageLoader::ImageLoader()
{

}


ImageLoader::~ImageLoader()
{

}


IResource* ImageLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	switch (info.GetType())
	{
	case EResourceType::RT_Texture2D:
	{
		Texture2D* texture = new Texture2D();
		archive.ArchiveLoad(*texture);
		return texture;
	}

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}

	return nullptr;
}


void ImageLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	switch (Resource->GetType())
	{
	case EResourceType::RT_Texture2D:
	{
		Texture2D* texture = static_cast<Texture2D*>(Resource);
		archive.ArchiveSave(*texture);
	}
		break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}
}


void ImageLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Texture2D);
	outRscTypes.push_back(RT_TextureCube);
}


} // End of namespace Raven

