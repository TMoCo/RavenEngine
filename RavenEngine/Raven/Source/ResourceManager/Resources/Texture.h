#pragma once



#include "ResourceManager/Resources/IResource.h"
#include "Utilities/Core.h"


#include "glm/vec2.hpp"
#include <stdint.h>



namespace Raven
{
	class RenderRscTexture;




	// Texture Formats.
	enum class ETextureFormat : uint32_t
	{
		// 
		R8,
		RGB24,
		RGBA32,

		//
		R_Float,
		RGB_Float,
	};



	// Texture Filter.
	enum class ETextureFilter : uint32_t
	{
		Nearest,
		Linear,
		TriLinear
	};


	// Texture Wrap.
	enum class ETextureWrap : uint32_t
	{
		Repeat,
		Mirror,
		ClampToEdge,
	};


	// Return information about a texture format.
	extern void GetFormatInfo(ETextureFormat format, int32_t& BPP);


	// ITexture:
	//		- base class for all texture resrouces.
	//
	class ITexture : public IResource
	{
		NOCOPYABLE(ITexture);

	public:
		// Construct.
		ITexture()
		{
			hasRenderResources = true;
			isGenMipmaps = false;
			renderRsc = NULL;
		}

		// Destruct.
		virtual ~ITexture() { }

		// Return the texture size.= width and height.
		inline const glm::ivec2& GetSize() const { return size; }

		// Return the texture format.
		inline ETextureFormat GetFormat() const { return format; }

		// Return the texture filter mode.
		inline ETextureFilter GetFilter() const { return filter; }

		// Return the texture wrap mode.
		inline ETextureWrap GetWrap() const { return wrap; }

		// Set the filter mode.
		inline void SetFitler(ETextureFilter inFilter) { filter = inFilter; }

		// Set the wrap mode.
		inline void SetWrap(ETextureWrap inWrap) { wrap = inWrap; }

		// Set gen mipmaps flag to true to generate one on load or next update.
		inline void SetGenMipmaps(bool val) { isGenMipmaps = val; }

		// Return texture render resource.
		inline RenderRscTexture* GetRenderRsc() const { return renderRsc.get(); }



		// Serialization Save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<IResource>(this));

			archive(
				size, 
				EnumAsInt<ETextureFormat>(format),
				EnumAsInt<ETextureFilter>(filter),
				EnumAsInt<ETextureWrap>(wrap),
				isGenMipmaps
			);
		}

		// Serialization Load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<IResource>(this));

			archive(
				size,
				EnumAsInt<ETextureFormat>(format),
				EnumAsInt<ETextureFilter>(filter),
				EnumAsInt<ETextureWrap>(wrap),
				isGenMipmaps
			);
		}

	protected:
		// Image size.
		glm::ivec2 size;

		// The texture format.
		ETextureFormat format;

		// The texture filter mode.
		ETextureFilter filter;

		// The texture wrap mode.
		ETextureWrap wrap;

		// if true will generate mipmaps on load or next update.
		bool isGenMipmaps;

		// Render Resrouce.
		Ptr<RenderRscTexture> renderRsc;
	};
}