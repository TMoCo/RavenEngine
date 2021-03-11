#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Resources/Texture2D.h"

namespace Raven
{
	// factory method for creating resources
	IResource* IResource::CreateResource(EResourceType type)
	{
		switch (type)
		{
		case Raven::EResourceType::RT_Image:
			return new Texture2D();
		default:
			return nullptr;
		}
	}
}