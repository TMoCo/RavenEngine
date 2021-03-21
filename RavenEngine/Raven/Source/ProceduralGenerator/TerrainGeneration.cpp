#include "TerrainGeneration.h"
#include <iostream>
#include <fstream>

namespace Raven {

	TerrainGeneration::TerrainGeneration()
	{
	}

	TerrainGeneration::~TerrainGeneration()
	{
	}

	void TerrainGeneration::Initialize()
	{
		Noise();
	}

	void TerrainGeneration::Destroy()
	{
	}

	void TerrainGeneration::Noise()
	{

		const int width = 128;
		const int height = 128;
		const int octaves = 4;
		float a = 0.1;
		float b = 1.0;

		// buffer to store noise data
		//GLubyte* data = new GLubyte[width * height * octaves];
		float data[width * height * octaves];
		
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
					float val = glm::perlin(p) / scale;
					sum += val;
					float result = (sum + 1.0f) / 2.0f;
					// store in texture buffer
					//data[((row * width + col) * octaves) + oct] = (GLubyte)(result * 255.0f);
					data[((row * width + col) * octaves) + oct] = result * 255.0f;
					//LOGI(data[((row * width + col) * octaves) + oct]);

					freq *= 2.0f; // double the frequency
					scale *= b; // next power of b
				}
			}
		}

		std::ostringstream outFile;

		outFile << "heightmap.ppm";

		std::ofstream out(outFile.str().c_str());

		out << "P3 " << width << " " << height << " 255\n";

		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				for (int oct = 0; oct < octaves; oct++)
				{
					if (oct == 3)
					{
						out << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << '\n';
					}
										
				}
			}
		}

		out.close();

		//GLuint texID;
		//glGenTextures(1, &texID);
		//glBindTexture(GL_TEXTURE_2D, texID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//delete [] data;
		
	}
}