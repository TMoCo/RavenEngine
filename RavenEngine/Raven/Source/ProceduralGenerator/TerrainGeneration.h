#pragma once

#include "IModule.h"
#include "Logger/Console.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>
#include <fstream>

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
		enum FileFormat
		{
			PNG, BMP, JPG, TGA, HDR
		};

		// initialise module
		virtual void Initialize() override;

		// destroy module
		virtual void Destroy() override;

		// create noise
		void Noise(int width, int height, FileFormat type);

		// write out image in the specified format
		void WriteImage(FileFormat type, int width, int height, const float* data);

	};

}