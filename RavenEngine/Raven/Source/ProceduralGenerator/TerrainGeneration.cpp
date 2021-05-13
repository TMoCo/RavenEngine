#include "TerrainGeneration.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


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



Ptr<HeightMap> TerrainGeneration::GenerateHeightMap(int32_t width, int32_t height)
{
	GenerateSquareGradient(width, height);
	GenerateNoise(width, height);

	const int octaves = 4;

	Ptr<HeightMap> heightMap(new HeightMap());
	heightMap->Allocate(width, height);


	// add heights from all octaves
	float sumHeight = 0;
	// compute the average height and store as RGB data
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			sumHeight = 0;

			for (int oct = 0; oct < octaves; oct++)
			{
				sumHeight += data[((row * width + col) * octaves) + oct];
			}


			sumHeight /= 4;
			heightMap->SetValue(row, col, sumHeight);
		}
	}

	float newHeight;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			// subtract square gradient from original height
			newHeight = heightMap->GetValue(row, col) - squareGradient[row * width + col];

			// set negative heights to 0
			newHeight = newHeight < 0 ? 0 : newHeight;

			// invert heights (render has 0 as the tallest)
			//newHeight = 1.0f - newHeight;


			heightMap->SetValue(row, col, newHeight);
		}
	}


	// deallocate memory
	free(squareGradient);
	free(data);

	return heightMap;
}



// Create a square gradient for making an island
void TerrainGeneration::GenerateSquareGradient(int width, int height)
{
	int halfWidth = width / 2;
	int halfHeight = height / 2;

	squareGradient = (float*)malloc(width * height * sizeof(float));

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
			value = 1.0f - value;

			// use a higher power (value^3) to increase the area of the black centre
			//value *= value * value;
			value *= value;

			squareGradient[row * width + col] = glm::smoothstep(0.0f, 1.0f, value);
		}
	}


	//WriteImage(FileFormat::PNG, width, height, squareGradient);
}



void TerrainGeneration::GenerateNoise(int width, int height)
{	
	const int octaves = 4;



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
				data[((row * width + col) * octaves) + oct] = result;

				freq *= freqFactor; // the frequency
				scale *= b; // next power of b
			}
		}
	}

}


bool TerrainGeneration::WriteHeightMap(HeightMap* map, const std::string& path)
{
	int32_t width = map->GetSize().x;
	int32_t height = map->GetSize().x;
	int32_t pixelCount = width * height;

	float* heightData = map->GetHeightMapData();
	uint8_t* data = (uint8_t*)malloc(width * height * sizeof(uint8_t));

	for (int32_t i = 0; i < pixelCount; ++i)
	{
		data[i] = (uint8_t)(heightData[i] * 255.0f);
	}

	int success = stbi_write_png(path.c_str(), width, height, 1, data, width);	// last parameter is stride in bytes

	free(data);
	return success;
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