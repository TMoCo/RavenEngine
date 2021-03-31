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
		FileFormat type = PNG;
		const int width = 128;
		const int height = 128;

		Noise(width, height, type);
	}

	void TerrainGeneration::Destroy()
	{
	}

	void TerrainGeneration::Noise(int width, int height, FileFormat type)
	{	
		const int octaves = 4;
		// seeds
		float a = 0.5;
		float b = 1.0;
		bool periodic = false;

		// buffer to store noise data
		//GLubyte* data = new GLubyte[width * height * octaves];
		float* data;
		data = (float*)malloc(width * height * octaves * sizeof(float));

		uint8_t* heightFields;
		heightFields = (uint8_t*)malloc(width * height * 4 * sizeof(uint8_t));

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
					//data[((row * width + col) * octaves) + oct] = (GLubyte)(result * 255.0f);
					data[((row * width + col) * octaves) + oct] = result * 255.0f;

					freq *= 2.0f; // double the frequency
					scale *= b; // next power of b
				}
			}
		}

		// add heights from all octaves
		float sumHeight = 0;

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
						//out << (int)sumHeight << ' ' << (int)sumHeight << ' ' << (int)sumHeight << '\n';
						sumHeight = 0;
						//out << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << '\n';
					}
				}
			}
		}

		WriteImage(type, width, height, heightFields);
		//out.close();
		free(data);

		//GLuint texID;
		//glGenTextures(1, &texID);
		//glBindTexture(GL_TEXTURE_2D, texID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//delete [] data;

	}

	void TerrainGeneration::WriteImage(FileFormat type, int width, int height, const uint8_t* data)
	{
		// write heights to different image formats
		switch (type)
		{
		case PNG:
			stbi_write_png("heightmap.png", width, height, 3, data, width * 3);	// last parameter is stride in bytes
			break;
		case BMP:
			stbi_write_bmp("heightmap.bmp", width, height, 3, data);
			break;
		case JPG:
			stbi_write_jpg("heightmap.jpg", width, height, 3, data, 100);	// int quality
			break;
		case TGA:
			stbi_write_tga("heightmap.tga", width, height, 3, data);
			break;
		}
	}
}