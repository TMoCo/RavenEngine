//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

#include "Utilities/Core.h"
#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscTerrain.h"

//
// A class for a Terrain, with height map and other features to come later
//

namespace Raven
{
	class Terrain : public IResource
	{
	public:
		Terrain(Texture2D* initHeight = nullptr) : IResource(EResourceType::RT_Terrain, true),
			heightMap(initHeight) 
		{
			if (heightMap->format != EGLFormat::R)
			{
				// Invalid image format for a height map (must be grayscale)
				throw std::runtime_error("Height map must be grayscale!");
			}
		}
		
		inline virtual ~Terrain()
		{
			if (heightMap)
			{
				// TODO: free data in memory manager
				delete heightMap;
			}
		}

		inline void LoadOnGPU()
		{
			if (!onGPU)
			{
				renderRscTerrain->LoadHeightMap(heightMap->width, heightMap->height, heightMap->data);
				onGPU = true;
			}
		}

		inline static EResourceType Type() noexcept { return EResourceType::RT_Terrain; } // return the resource type

		// store heights in a texture
		Texture2D* heightMap; 

		// textures 
		std::vector<Texture2D*> textures;

		// interface with renderer
		RenderRscTerrain* renderRscTerrain = nullptr; 

		NOCOPYABLE(Terrain);
	};
}
