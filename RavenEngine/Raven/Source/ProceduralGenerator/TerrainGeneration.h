#pragma once

//#include <glew-2.1.0/include/GL/glew.h>
#include "IModule.h"

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>
#include <fstream>
#include "Logger/Console.h"

namespace Raven {

	class TerrainGeneration : public IModule
	{
	public:
		// constructor
		TerrainGeneration();

		// destructor
		~TerrainGeneration();

		// return the type of the module
		static EModuleType GetModuleType() { return MT_ProceduralGenerator; }


	private:
		// initialise module
		virtual void Initialize() override;

		// destroy module
		virtual void Destroy() override;

		// create noise
		void Noise();


	};

}