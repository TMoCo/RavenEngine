#pragma once




#include "IRenderResource.h"
#include "Render/OpenGL/GLTypes.h"

#include "Utilities/Core.h"
#include "glm/vec2.hpp"



namespace Raven
{
	class GLTexture;



	//
	//
	class RenderRscTexture : public IRenderResource
	{
	public:
		// Construct.
		RenderRscTexture();

		// Destruct.
		~RenderRscTexture();

		//  Load a render texture resource.
		void Load(EGLTexture type, EGLFormat format, const glm::ivec2&	size, void* data);

		// Return the OpenGL texture.
		inline const Ptr<GLTexture>& GetTexture() { return texture; }

	private:
		// The OpenGL Texture.
		Ptr<GLTexture> texture;
	};

}


