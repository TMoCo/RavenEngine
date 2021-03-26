//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Scene/Component/Terrain.h"

//
// A class for Terrain and their related data such as heightmaps, textures, later materials, entity placements...
//

namespace Raven
{
	void Terrain::LoadOnGpu()
	{
		// load the resources
		terrain.get()->LoadOnGPU();
		for (auto& texturePair : textures)
		{
			texturePair.second->LoadOnGpu();
		}
	}

	void Terrain::SetHeightMap(Texture2D* heightMap)
	{
		if (TerrainRsc::IsValidHeightMap(heightMap))
		{
			terrain->heightMap = heightMap;
		}
	}

	// return shared pointer to a mesh resource
	std::shared_ptr<Texture2D> Terrain::GetTexture(const std::string& name)
	{
		auto iter = textures.find(name);
		if (iter == textures.end())
		{
			return nullptr;
		}
		else
		{
			return std::shared_ptr<Texture2D>(iter->second);
		}
	}

	// get all textures
	std::vector<std::shared_ptr<Texture2D>>& Terrain::GetTextures()
	{
		// make a vector with the same number of textures as in the map
		std::vector<std::shared_ptr<Texture2D>> tex(textures.size());
		auto texIter = tex.begin();
		for (auto& texturePair : textures)
		{
			*texIter = std::shared_ptr<Texture2D>(texturePair.second);// set tex element to same as in texture map
			texIter = std::next(texIter); // increment vector iterator
		}
		return tex;
	}

	// add a texture to the map
	void Terrain::AddTexture(const std::string& name, Texture2D* newTexture)
	{
		textures.insert(std::make_pair(name, newTexture));
	}
}