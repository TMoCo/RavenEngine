#pragma once





#include "IRenderResource.h"




namespace Raven
{
	class GLTexture;



	//
	//
	class RenderTextureResource
	{
	public:
		// Construct.
		RenderTextureResource();

		// Destruct.
		~RenderTextureResource();

		

	private:
		// The OpenGL Texture.
		GLTexture* tex;
	};

}


