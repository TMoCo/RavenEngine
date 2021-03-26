//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

#include "ResourceManager/Resources/Texture2D.h"
#include "ResourceManager/Resources/TerrainRsc.h"

//
// A class for Terrain component and their related data such as heightmaps, textures, later materials, entity placements...
//

namespace Raven
{
	class Terrain
	{
	public:
		Terrain() {}
		// no destructor as resource lifetime is not decided by the component

		void LoadOnGpu();

		// return shared pointer to a mesh resource
		std::shared_ptr<Texture2D> GetTexture(const std::string& name);
		// get all textures
		std::vector<std::shared_ptr<Texture2D>>& GetTextures();

		void SetHeightMap(Texture2D* heightMap);

		void AddTexture(const std::string& name, Texture2D* newTexture);
	
	private:
		std::shared_ptr<TerrainRsc> terrain;
		// textures {name, texture} eg: {"grass", &resource}
		std::map<std::string, Texture2D*> textures;
	};
}