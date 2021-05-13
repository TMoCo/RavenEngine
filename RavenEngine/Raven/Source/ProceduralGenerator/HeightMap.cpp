#include "HeightMap.h"




namespace Raven {



HeightMap::HeightMap()
{

}


HeightMap::~HeightMap()
{

}


void HeightMap::Allocate(int32_t width, int32_t height)
{
	size = glm::ivec2(width, height);
	heightField.Allocate(width * height * sizeof(float));
}


void HeightMap::GenerateTexture()
{
	// Convert the height map into gl texture format.
	int32_t numPixels = size.x * size.y;
	const float* heightData = heightField.GetData();
	uint8_t* data = (uint8_t*)malloc(size.x * size.y * sizeof(uint8_t));

	for (int32_t i = 0; i < numPixels; ++i)
	{
		data[i] = (uint8_t)(heightData[i] * 255.0f);
	}

	// Create Texture.
	heightmapTexture = Ptr<DynamicTexture>(new DynamicTexture());
	heightmapTexture->SetTexType(EDynamicTexture::Tex2D);
	heightmapTexture->SetGenMipmaps(true);
	heightmapTexture->SetFitler(ETextureFilter::Linear);
	heightmapTexture->SetWrap(ETextureWrap::Mirror);
	heightmapTexture->UpdateData(size, ETextureFormat::R8, data);


	// Free...
	free(data);
}



}

