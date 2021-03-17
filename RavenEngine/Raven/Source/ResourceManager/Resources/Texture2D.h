#pragma once

#include <stdint.h> // uint8_t

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscTexture.h"
#include "Utilities/Core.h"

//
// A class for a 2D texture resource
//

namespace Raven
{
	class Texture2D : public IResource
	{
	public:
		Texture2D(size_t initWidth, size_t initHeight) : IResource(EResourceType::RT_Image, true),
			width(initHeight), height(initHeight) {}
		// TODO: free data on destruction
		virtual ~Texture2D() = default;

		inline static EResourceType Type() noexcept { return EResourceType::RT_Image; } // return the resource type

		size_t height; // image dimensions
		size_t width;
		
		uint8_t* data = nullptr; // image data (sizeof(uint8_t) * height * width))

		RenderRscTexture* renderResource = nullptr; // interface with renderer

		NOCOPYABLE(Texture2D);
	};
}