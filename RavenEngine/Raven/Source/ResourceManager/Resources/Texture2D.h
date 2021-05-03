//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once



#include "Texture.h"





namespace Raven
{

	// The Texture2D Data.
	class Texture2DData
	{
	private:
		// Texture Buffer.
		uint8_t* data;

		// The size of the data in bytes.
		uint32_t size;

	public:
		// Construct Null Data.
		Texture2DData()
			: data(nullptr)
			, size(0)
		{

		}

		// Destruct.
		~Texture2DData()
		{
			free(data);
		}

		// Allocate Texture Data.
		// @param size: the size of the data to allocate in bytes.
		inline void Allocate(uint32_t allocateSize)
		{
			RAVEN_ASSERT(data == nullptr, "Invalid Operation, trying to reallocate texture data.");
			data = (uint8_t*)malloc(allocateSize);
			size = allocateSize;
		}

		// Set Image Data.
		void SetData(uint32_t inOffset, uint32_t inSize, uint8_t* inData)
		{
			RAVEN_ASSERT(size >= (inOffset + inSize), "Invalid Operation, trying to reallocate texture data.");
			memcpy(data + inOffset, inData, inSize);
		}

		// Return the texture data.
		inline uint8_t* GetData() { return data; }
		inline const uint8_t* GetData() const { return data; }

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




	// Texture2D:
	//    - 
	//
	class Texture2D : public ITexture
	{
	public:
		// Construct.
		Texture2D();

		// Destruct.
		virtual ~Texture2D();

		// return the resource type
		inline static EResourceType StaticGetType() noexcept { return EResourceType::RT_Texture2D; }

		// Create/Load the texture render resrouce.
	  virtual void LoadRenderResource() override;

		// Update the texture render resrouce.
		virtual void UpdateRenderResource() override;

		// Set a new Image 2D data to this texture.
	  void SetImageData(ETextureFormat imgFormat, const glm::ivec2& imgSize, uint8_t* imgData);

		// Return texture data.
		inline const Texture2DData& GetData() const { return data; }

		// Serialization Load.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<ITexture>(this));

			// Save data as binary.
			uint32_t dataSize = data.GetSize();
			archive(dataSize);

			if (dataSize != 0)
			{
				SaveCompressed(archive, dataSize, data.GetData());
			}
		}

		// Serialization Save.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<ITexture>(this));

			// Save data as binary.
			uint32_t dataSize = 0;
			archive(dataSize);

			if (dataSize != 0)
			{
				data.Allocate(dataSize);
				LoadCompressed(archive, dataSize, data.GetData());
			}
		}


	private:
		// The Texture Data.
		Texture2DData data;
	};
}