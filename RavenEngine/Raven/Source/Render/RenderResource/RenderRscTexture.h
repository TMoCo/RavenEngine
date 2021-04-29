#pragma once




#include "IRenderResource.h"
#include "Render/OpenGL/GLTypes.h"
#include "ResourceManager/Resources/Texture.h"

#include "Utilities/Core.h"
#include "glm/vec2.hpp"



namespace Raven
{
	class GLTexture;




	// RenderRscTexture:
	//		-
	//
	class RenderRscTexture : public IRenderResource
	{
	public:
		// Construct.
		RenderRscTexture();

		// Destruct.
		~RenderRscTexture();

		// Return the OpenGL texture.
		inline const Ptr<GLTexture>& GetTexture() { return texture; }

		// Load a render texture resource.
		void Load(EGLTexture type, EGLFormat format, const glm::ivec2& size, void* data);

		// Create Texture 2D Render Resrouce.
		static RenderRscTexture* CreateTexture2D(ETextureFormat format, ETextureFilter filter, ETextureWrap wrap,
			const glm::ivec2& size, uint8_t* data, bool isGenMipmaps);

		// Update a single texture level and layer.
		void UpdateData(int32_t level, int32_t layer, const glm::ivec2& size, uint8_t* data);

		// Update
		void UpdateParamters(ETextureFilter filter, ETextureWrap wrap);

	public:
		// Conver to format to opengl format.
		static EGLFormat ToGLType(ETextureFormat format);

		// Conver to format to opengl filter.
		static EGLFilter ToGLType(ETextureFilter filter);

		// Conver to format to opengl wrap.
		static EGLWrap ToGLType(ETextureWrap wrap);

	private:
		// The OpenGL Texture.
		Ptr<GLTexture> texture;
	};

}


