#pragma once

#include "Logger/Console.h"
#include "HeightMap.h"

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

	class TerrainGeneration
	{
	public:
		// buffer to store squareGradient data 
		float* squareGradient;

		// buffer to store noise data
		float* data;

		// constructor
		TerrainGeneration();

		// destructor
		~TerrainGeneration();

		// initialise module
		void Initialize();

		// destroy module
		void Destroy();

		// Generate terrain heightmap.
		Ptr<HeightMap> GenerateHeightMap(int32_t width, int32_t height);

		// create a square gradient for island
		void GenerateSquareGradient(int width, int height);

		// create terrain noise
		void GenerateNoise(int width, int height);

		// write out image in the specified format
		bool WriteHeightMap(HeightMap* map, const std::string& path);

		// write out image in the specified format
		void WriteImage(FileFormat type, int width, int height, const uint8_t* data);

	private:
		// --- -- - --- -- - --- -- - --
		//       Gen-Paramters
		// --- - -- - -- - -- - -- - ---

		// higher value increases bumpiness; originally 0.5
		float a = 7.5;

		// a needs to be much higher than b for a bumpy terrain
		// higher value increases smoothness; originally 1.0
		float b = 1.5;

		// frequency factor, scale frequancy each octave.
		float freqFactor = 2.0f;

		// Offset to the noise seed.
		glm::vec2 seedOffset = glm::vec2(0.0f, 0.0f);
	};

}