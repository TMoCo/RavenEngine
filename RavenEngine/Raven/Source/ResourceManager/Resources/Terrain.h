//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

#include "glm/glm.hpp"

#include "Utilities/Core.h"
#include "ResourceManager/Resources/IResource.h" 
#include "ResourceManager/Resources/Texture2D.h" // for height map
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
			heightMap(initHeight) // vector for textures, may want to swap container to a map
		{
			if (!IsValidHeightMap(initHeight))
			{
				initHeight = nullptr;
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
				// load the height map data
				renderRscTerrain->LoadHeightMap(heightMap->width, heightMap->height, heightMap->data);
				onGPU = true;
			}
		}

		static bool IsValidHeightMap(Texture2D* heightMap)
		{
			if (heightMap->format != EGLFormat::R)
			{
				return false;
			}
			return true;
		}

		inline static EResourceType Type() noexcept { return EResourceType::RT_Terrain; } // return the resource type

		// store heights in a texture
		Texture2D* heightMap;

		// interface with renderer
		RenderRscTerrain* renderRscTerrain = nullptr; 

		NOCOPYABLE(Terrain);
	};
}
