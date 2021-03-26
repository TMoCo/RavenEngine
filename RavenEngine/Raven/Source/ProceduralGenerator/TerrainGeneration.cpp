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
		// seeds
		float a = 0.5;
		float b = 1.0;
		bool periodic = false;

		// buffer to store noise data
		//GLubyte* data = new GLubyte[width * height * octaves];
		//float data[width * height * octaves];
		float* data;
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
					//data[((row * width + col) * octaves) + oct] = (GLubyte)(result * 255.0f);
					data[((row * width + col) * octaves) + oct] = result * 255.0f;

					freq *= 2.0f; // double the frequency
					scale *= b; // next power of b
				}
			}
		}

		// change file name
		//std::ostringstream outFile;
		//outFile << "height0.ppm";
		//outFile.str().c_str());

		std::ofstream out("height.ppm"); 

		out << "P3 " << width << " " << height << " 255\n";

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
						out << (int)sumHeight << ' ' << (int)sumHeight << ' ' << (int)sumHeight << '\n';
						sumHeight = 0;
						//out << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << ' ' << (int)data[((row * width + col) * octaves) + oct] << '\n';
					}
				}
			}
		}

		out.close();
		free(data);

		//GLuint texID;
		//glGenTextures(1, &texID);
		//glBindTexture(GL_TEXTURE_2D, texID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//delete [] data;

	}
}