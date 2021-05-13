#pragma once



#include "IModule.h"
#include "Utilities/Core.h"

#include "TerrainGeneration.h"



namespace Raven 
{
	class Scene;


	// ProceduralGenerator
	//    - Handle generating a new scene procedurally.
	//
	class ProceduralGenerator : public IModule
	{
	public:
		// Consttruct.
		ProceduralGenerator();

		// Destruct.
		~ProceduralGenerator();

		// initialise module
		virtual void Initialize() override;

		// destroy module
		virtual void Destroy() override;

		// Return the type of the module
		static EModuleType GetModuleType() { return MT_ProceduralGenerator; }

		// Return Terrain Generation instance.
		inline TerrainGeneration* GetTerrainGen() { return terrainGen.get(); }

		// Generate a new scene procedurally.
		Scene* GenerateNewScene(const glm::vec2& size, const glm::vec2& height);

	private:
		// The Terrain Generation object.
		Ptr<TerrainGeneration> terrainGen;
	};


}

