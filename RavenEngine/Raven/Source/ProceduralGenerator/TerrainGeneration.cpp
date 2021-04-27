#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "TerrainGeneration.h"

namespace Raven {

	TerrainGeneration::TerrainGeneration()
	{
	}

	TerrainGeneration::~TerrainGeneration()
	{
	}

	void TerrainGeneration::Initialize()
	{
	}

	void TerrainGeneration::Destroy()
	{
	}

	void TerrainGeneration::GenerateTerrain(int width, int height, FileFormat type)
	{
		GenerateSquareGradient(width, height);
		GenerateNoise(width, height);

		const int octaves = 4;

		uint8_t* heightFields;
		heightFields = (uint8_t*)malloc(width * height * 4 * sizeof(uint8_t));

		// add heights from all octaves
		float sumHeight = 0;
		// compute the average height and store as RGB data
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				for (int oct = 0; oct < octaves; oct++)
				{
					sumHeight += data[((row * width + col) * octaves) + oct];

					if (oct == 3)
					{
						sumHeight /= 4;

						heightFields[(row * width + col) * 3] = sumHeight;
						heightFields[((row * width + col) * 3) + 1] = sumHeight;
						heightFields[((row * width + col) * 3) + 2] = sumHeight;
						sumHeight = 0;
						//out << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << '\n';
					}
				}
			}
		}

		float newHeight;
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				// subtract square gradient from original height
				newHeight = heightFields[(row * width + col) * 3] - squareGradient[(row * width + col) * 3];

				// set negative heights to 0
				newHeight = newHeight < 0 ? 0 : newHeight;

				// invert heights (render has 0 as the tallest)
				newHeight = 255 - newHeight;

				heightFields[(row * width + col) * 3] = newHeight;
				heightFields[((row * width + col) * 3) + 1] = newHeight;
				heightFields[((row * width + col) * 3) + 2] = newHeight;
			}
		}

		// write heights to image file
		WriteImage(type, width, height, heightFields);

		// deallocate memory
		free(squareGradient);
		free(data);
		free(heightFields);
	}

	// Create a square gradient for making an island
	void TerrainGeneration::GenerateSquareGradient(int width, int height)
	{
		int halfWidth = width / 2;
		int halfHeight = height / 2;

		squareGradient = (uint8_t*)malloc(width * height * 3 * sizeof(uint8_t));

		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				int x = col;
				int y = row;

				float value;

				x = x > halfWidth ? width - x : x;
				y = y > halfHeight ? height - y : y;

				int smaller = x < y ? x : y;
				value = smaller / (float)halfWidth;

				// invert so the edges have the highest value
				value = 1 - value;
				// use a higher power (value^3) to increase the area of the black centre
				//value *= value * value;
				//value *= value;

				value = value * 255;

				squareGradient[(row * width + col) * 3] = value;
				squareGradient[((row * width + col) * 3) + 1] = value;
				squareGradient[((row * width + col) * 3) + 2] = value;
			}
		}

		//WriteImage(FileFormat::PNG, width, height, squareGradient);
	}

	void TerrainGeneration::GenerateNoise(int width, int height)
	{	
		const int octaves = 4;
		// seeds
		float a = 7.5;	// higher value increases bumpiness; originally 0.5
		float b = 1.5;	// higher value increases smoothness; originally 1.0
		// a needs to be much higher than b for a bumpy terrain

		bool periodic = false;	// for seamless noise

		data = (float*)malloc(width * height * octaves * sizeof(float));

		float xFactor = 1.0f / (width - 1);
		float yFactor = 1.0f / (height - 1);

		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				float x = xFactor * col;
				float y = yFactor * row;
				float sum = 0.0f;
				float freq = a;
				float scale = b;

				// compute the sum for each octave
				for (int oct = 0; oct < octaves; oct++)
				{
					glm::vec2 p(x * freq, y * freq);
					float val = 0.0f;
					// periodic for seamless noise
					if (periodic) {
						val = glm::perlin(p, glm::vec2(freq)) / scale;
					}
					// normal
					else {
						val = glm::perlin(p) / scale;
					}
					sum += val;
					float result = (sum + 1.0f) / 2.0f;
					// store in texture buffer
					data[((row * width + col) * octaves) + oct] = result * 255.0f;

					freq *= 2.0f; // double the frequency
					scale *= b; // next power of b
				}
			}
		}
	}

	void TerrainGeneration::WriteImage(FileFormat type, int width, int height, const uint8_t* data)
	{
		// write heights to different image formats
		switch (type)
		{
		case FileFormat::PNG:
			stbi_write_png("heightmap.png", width, height, 3, data, width * 3);	// last parameter is stride in bytes
			break;
		case FileFormat::BMP:
			stbi_write_bmp("heightmap.bmp", width, height, 3, data);
			break;
		case FileFormat::JPG:
			stbi_write_jpg("heightmap.jpg", width, height, 3, data, 100);	// int quality
			break;
		case FileFormat::TGA:
			stbi_write_tga("heightmap.tga", width, height, 3, data);
			break;
		}
	}
}