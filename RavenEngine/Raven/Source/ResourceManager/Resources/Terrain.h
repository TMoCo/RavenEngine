#pragma once

#include "glm/glm.hpp"

#include "Utilities/Core.h"
#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscTerrain.h"

namespace Raven
{
	class Terrain : public Mesh
	{
	public:
		Terrain(Texture2D* initHeight = nullptr) : IResource(EResourceType::RT_Terrain, true),
			heightMap(initHeight) {}
		
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
				renderRscTerrain.LoadHeightMap(heightMap->width, heightMap->height, heightMap->data);
				onGPU = true;
			}
		}

		inline static EResourceType Type() noexcept { return EResourceType::RT_Terrain; } // return the resource type

		Texture2D* heightMap; // store heights in a texture
		// is a mesh so also has verts, normals, texCoords, indices

		RenderRscTerrain renderRscTerrain; // interface with renderer

		NOCOPYABLE(Terrain);
	};
}
