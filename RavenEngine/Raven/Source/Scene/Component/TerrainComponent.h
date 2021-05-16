//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

#include "ResourceManager/Resources/Texture2D.h"
#include "ResourceManager/Resources/Terrain.h"
#include "Component.h"






namespace Raven
{

	// Represent The terrain in the scene.
	//     - 
	//
	class TerrainComponent : public Component
	{
	public:
		// Construct.
		TerrainComponent();

		// Destruct.
		~TerrainComponent();

		// Set The Terrain.
		void SetTerrainResource(Ptr<Terrain> newTerrain);

		// Return the terrain resrouce.
		Ptr<Terrain> GetTerrain() const { return terrain; }

		// Serialization Save
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));


		}

		// Serialization Load
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));


		}

	private:
		// The Terrain.
		Ptr<Terrain> terrain;

		//


	};
}