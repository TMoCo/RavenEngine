#pragma once



#include "Utilities/Core.h"
#include "ResourceManager/Resources/DynamicTexture.h"



namespace Raven
{
	// HeightMapData:
	//    - 
	template<class TData>
	struct HeightMapData
	{
	private:
		// Texture Buffer.
		TData* data;

		// The size of the data in bytes.
		uint32_t size;

	public:
		// Construct Null Data.
		HeightMapData()
			: data(nullptr)
			, size(0)
		{

		}

		// Destruct.
		~HeightMapData()
		{
			free(data);
		}

		// Allocate Texture Data.
		// @param size: the size of the data to allocate in bytes.
		inline void Allocate(uint32_t allocateSize)
		{
			RAVEN_ASSERT(data == nullptr, "Invalid Operation, trying to reallocate data.");
			data = (TData*)malloc(allocateSize);
			size = allocateSize;
		}

		// Set Image Data.
		void SetData(uint32_t inOffset, uint32_t inSize, TData* inData)
		{
			RAVEN_ASSERT(size >= (inOffset + inSize), "Invalid Operation, trying to reallocate data.");
			memcpy(data + inOffset, inData, inSize);
		}

		// Return the texture data.
		inline TData* GetData() { return data; }
		inline const TData* GetData() const { return data; }

		// Return the allocated size for the image data.
		inline const uint32_t& GetSize() const { return size; }

		// Reset/Free the data.
		inline void Reset()
		{
			free(data);
			data = nullptr;
			size = 0;
		}
	};



	// HeightMap:
	//    - 
	//
	class HeightMap
	{
	public:
		// Construct.
		HeightMap();

		// Destruct.
		~HeightMap();

		// Allocate height map data.
		void Allocate(int32_t width, int32_t height);

		// Set the height value.
		void SetValue(int32_t x, int32_t y, float value) { heightField.GetData()[x + y * size.x] = value; }

		// Return the height value.
		inline float& GetValue(int32_t x, int32_t y) { return heightField.GetData()[x + y * size.x]; }
		inline const float& GetValue(int32_t x, int32_t y) const { return heightField.GetData()[x + y * size.x]; }

		// Return height field data.
		inline float* GetHeightMapData() { return heightField.GetData(); }
		inline const float* GetHeightMapData() const { return heightField.GetData(); }

		// Return height field size.
		inline const glm::ivec2& GetSize() const { return size; }

		// Return height map texture used for rendering.
		inline DynamicTexture* GetHeightmapTexture() { return heightmapTexture.get(); }
		inline const DynamicTexture* GetHeightmapTexture() const { return heightmapTexture.get(); }

		// Generate Height Map Dynamic Texture to be used in renderings.
		void GenerateTexture();

	private:
		// The width and height of the height map.
		glm::ivec2 size;

		// The height field in floating points.
		HeightMapData<float> heightField;

		// The height map texture we are going to use for rendering.
		Ptr<DynamicTexture> heightmapTexture;
	};

}