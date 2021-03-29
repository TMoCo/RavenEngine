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
		Terrain(const std::string& fileName);
		// no destructor as resource lifetime is not decided by the component

		void LoadOnGpu();

		// return shared pointer to a mesh resource
		std::shared_ptr<Texture2D> GetTexture(const std::string& name);
		// get all textures
		std::vector<std::shared_ptr<Texture2D>>& GetTextures();

		void SetHeightMap(Texture2D* heightMap);

		void AddTexture(const std::string& name, Texture2D* newTexture);

		// serialization save and load
		template<typename Archive>
		void save(Archive& archive) const
		{
			// save the terrain height map and associated texture paths
			archive(cereal::make_nvp("HeightMap"), heightMapPath, cereal::make_nvp("Textures"), GetTexturePaths());
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			std::vector<std::string> paths;
			// load the paths for terrain height map and textures from the serialized file
			archive(cerreal::make_nvp("HeightMap"), heightMapPath, cereal::make_nvp("Textures"), paths);
			// now load the data 
			LoadHeightMap(heightMapPath);
			for (auto& path : paths)
			{
				LoadTexture(path);
			}			
		}

		bool HasHeightMap() const;

		void LoadHeightMap(const std::string& path);
		// load a texture and give it a name
		void LoadTexture(const std::string& path);

	private:
		std::vector<std::string> GetTexturePaths();

		// file name string for accessing data from resource manager
		std::string heightMapPath;

		std::shared_ptr<TerrainRsc> terrain;
		// textures {"path", ptr}
		std::map<std::string, Texture2D*> textures;
	
	};
}