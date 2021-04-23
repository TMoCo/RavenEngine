//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

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
	// one byte = 8 bits
	typedef uint8_t byte;

	class Texture2D : public IResource
	{
	public:
		Texture2D(size_t initWidth = 0, size_t initHeight = 0, EGLFormat format = EGLFormat::None, byte* initData = nullptr) 
			: IResource(EResourceType::RT_Image, true), 
			width(initWidth), height(initHeight), format(format), data(initData) 
		{
			renderRscTexture = NULL;
		}

		inline virtual ~Texture2D()
		{
			if (data)
			{
				// TODO: free data from memory manager when implemented
				delete[] data; // delete array of texture data
			}
		}

		inline void LoadOnGpu()
		{
			if (!onGPU)
			{
				renderRscTexture->Load(EGLTexture::Texture2D, EGLFormat::RGBA, glm::ivec2(width, height), data); // call interface method
				onGPU = true;
			}
		}

		// return the resource type
		inline static EResourceType Type() noexcept { return EResourceType::RT_Image; } 

		size_t height;	  // image dimensions
		size_t width;
		EGLFormat format; // image format
		
		byte* data; // image data should be sizeof(byte) * height * width

		RenderRscTexture* renderRscTexture; // interface with renderer
		NOCOPYABLE(Texture2D);
	};
}