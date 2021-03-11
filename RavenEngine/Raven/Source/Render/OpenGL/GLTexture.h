#pragma once






#include "GLTypes.h"





namespace Raven
{

	//
	//
	class GLTexture
	{
	private:
		// Construct.
		GLTexture();

	public:
		// Destruct.
		~GLTexture();

		// Create texture.
		static GLTexture* Create(EGLTexture type, EGLFormat format);

		// Create a texture and allocate data of size.
		static GLTexture* Create2D(EGLFormat format, int width, int height, EGLFilter filter = EGLFilter::Nearest, EGLWrap wrap = EGLWrap::ClampToEdge);

		// Create a texture and copy the data of the image.
		static GLTexture* Create2D(EGLFormat format, int width, int height, const void* data, EGLFilter filter = EGLFilter::Nearest, EGLWrap wrap = EGLWrap::ClampToEdge);

		// Update Texture data depending on its type
		// Note: this assume that the texture is currently bounded.
		void UpdateTexData(int level, int newWidth, int newHeight, const void* data);

	  // Update Texture data depending on its type and with multiple layers
		// Note: this assume that the texture is currently bounded.
		void UpdateTexData(int level, int newWidth, int newHeight, int numLayers, const void* data);

		// Update Texture parameters
	  // Note: this assume that the texture is currently bounded.
		void UpdateTexParams();

		// Generate texture mipmaps using glGenerateMipmaps
		// Note: this assume that the texture is currently bounded.
		void GenerateMipmaps();

		// Bind the texture to its target type.
		void Bind();

		// Unbind all buffers from target type.
		void Unbind();

		// Pixel information based on the texture format.
		static void GetPixelInfo(EGLFormat format, GLENUM& pixelFormat, GLENUM& pixelType, int& alignment);
		 
		// Return the opengl id of the buffer.
		inline GLUINT GetID() const { return id; }

		// Return texture type.
		inline EGLTexture GetType() const { return type; }

		// Return texture format.
		inline EGLFormat GetFormat() const { return format; }

		// Return texture format.
		inline EGLFilter GetFilter() const { return filter; }

		// Bind this texture and set it as active for TEXTURE(i)
		void Active(int i);

	private:
		// OpenGL Object ID.
		GLUINT id;

		// texture type.
		EGLTexture type;

		// texture format.
		EGLFormat format;

		// sample filter
		EGLFilter filter;

		// sample wrap
		EGLWrap wrap;

		// Texture width.
		int width;

		// Texture height.
		int height;
	};

}

