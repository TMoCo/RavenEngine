#pragma once





#include "IRenderResource.h"




namespace Raven
{
	class GLTexture;



	//
	//
	class RenderRscTexture : public IRenderResource
	{
	public:
		// Construct.
		RenderRscTexture() {}

		// Destruct.
		~RenderRscTexture() {}

		//
		void LoadTexture() {}

	private:
		// The OpenGL Texture.
		GLTexture* tex;
	};

}


