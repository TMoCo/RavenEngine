#pragma once

#include <stdint.h> // uint8_t

#include "ResourceManager/Resources/IResource.h"
#include "Utilities/Core.h"

//
// A class for a 2D texture resource
//

namespace Raven
{
	class RenderTextureResource;

	class Texture2D : public IResource
	{
	public:
		Texture2D(size_t initWidth, size_t initHeight) : IResource(EResourceType::RT_Image),
			width(initHeight), height(initHeight) {}

		size_t height; // image dimensions
		size_t width;
		
		uint8_t* data = nullptr; // image data

		RenderTextureResource* renderResource = nullptr; // interface with renderer

		NOCOPYABLE(Texture2D);
	};
}