//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Engine.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/Component/Terrain.h"

//
// A class for Terrain and their related data such as heightmaps, textures, later materials, entity placements...
//

namespace Raven
{
	Terrain::Terrain(const std::string& path)
	{
		LoadHeightMap(path);
	}

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
			*texIter = std::shared_ptr<Texture2D>(texturePair.second); // set tex element to same as in texture map
			texIter = std::next(texIter); // increment vector iterator
		}
		return tex;
	}

	// add a texture to the map
	void Terrain::AddTexture(const std::string& name, Texture2D* newTexture)
	{
		textures.insert(std::make_pair(name, newTexture));		
	}

	bool Terrain::HasHeightMap() const
	{
		// true if terrain height map is not nullptr
		if (terrain->heightMap != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Terrain::LoadHeightMap(const std::string& path)
	{
		auto res = Engine::Get().GetModule<ResourceManager>();
		res->LoadResource<Texture2D>(path);
		// once the texture has been loaded, try to set it as height map (must be gray scale, will be nullptr otherwise)
		SetHeightMap(res->GetResource<Texture2D>(path).get());
		// change the file name if height map was accepted
		if (HasHeightMap())
		{
			heightMapPath = path;
		}
	}

	// load a texture and give it a name
	void Terrain::LoadTexture(const std::string& path)
	{
		auto res = Engine::Get().GetModule<ResourceManager>();
		// use return value to determine if need to process further
		if (res->LoadResource<Texture2D>(path))
		{
			// insert the texture data if loaded
			auto tex = res->GetResource<Texture2D>(path);
			AddTexture(path, tex.get());
		}
	}

	std::vector<std::string> Terrain::GetTexturePaths()
	{
		std::vector<std::string> paths(textures.size());
		auto iter = paths.begin(); // start at the beginning of the vector
		for (auto& tex : textures)
		{
			// put the key (path) into the vector
			*iter = tex.first;
			std::next(iter); // increment the vector iterator
		}
		return paths;
	}
}