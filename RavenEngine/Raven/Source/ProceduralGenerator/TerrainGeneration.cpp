#include "TerrainGeneration.h"
#include <iostream>

namespace Raven {

	TerrainGeneration::TerrainGeneration()
	{
	}

	TerrainGeneration::~TerrainGeneration()
	{
	}

	void TerrainGeneration::Initialize()
	{
		LOGE("Generate terrain");
		Noise();
	}

	void TerrainGeneration::Destroy()
	{
	}

	void TerrainGeneration::Noise()
	{
		
		LOGE(glm::perlin(glm::vec2()));
	}
}