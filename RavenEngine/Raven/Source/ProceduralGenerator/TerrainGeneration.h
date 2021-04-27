#pragma once

#include "IModule.h"
#include "Logger/Console.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>
#include <fstream>
#include <vector>

namespace Raven {

	enum class FileFormat
	{
		PNG, BMP, JPG, TGA
	};

	class TerrainGeneration : public IModule
	{
	public:
		uint8_t* squareGradient;
		// buffer to store noise data
		float* data;

		// constructor
		TerrainGeneration();

		// destructor
		~TerrainGeneration();

		// return the type of the module
		static EModuleType GetModuleType() { return MT_ProceduralGenerator; }

		// initialise module
		virtual void Initialize() override;

		// destroy module
		virtual void Destroy() override;

		void GenerateTerrain(int width, int height, FileFormat type);

		// create a square gradient for island
		void GenerateSquareGradient(int width, int height);

		// create terrain noise
		void GenerateNoise(int width, int height);

		// write out image in the specified format
		void WriteImage(FileFormat type, int width, int height, const uint8_t* data);

	};

}