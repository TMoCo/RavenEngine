//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

#include "ResourceManager/Resources/Texture2D.h"

//
// A class for Terrain and their related data such as heightmaps, textures, later materials, entity placements...
//

namespace Raven
{
	class Terrain
	{
	public:
		Terrain() {}

		~Terrain();

		void LoadOnGpu();

		// return shared pointer to a mesh resource
		std::shared_ptr<Texture2D> GetMesh(const std::string& name);
		// pointer to the vector of meshes
		auto& GetMeshes();

		void AddTexture(const std::string& name, Texture2D* newTexture);
		void AddTextures(const std::vector<std::pair<std::string, std::shared_ptr<Texture2D>>>& inputTextures);

	private:
		std::shared_ptr<Texture2D> heightMap;

		std::map<std::string, std::shared_ptr<Texture2D>> textures;
	};
}