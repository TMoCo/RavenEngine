#pragma once

#include "ResourceManager/Resources/IResource.h"

//
// A class for a 2D texture resource
//

namespace Raven
{
	class TextureRenderResource;

	class Texture2D : public IResource
	{
	public:
		Texture2D() : IResource(EResourceType::RT_Image) {}

		size_t height;
		size_t width;

		TextureRenderResource* renderResource;
	};
}