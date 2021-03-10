#pragma once




#include "GLTypes.h"





namespace Raven
{
	//
	//
	class GLRenderBuffer
	{
	private:
		// Construct.
		GLRenderBuffer();

	public:
		// Destruct. 
		GLRenderBuffer();

		// Return the opengl id of the buffer.
		inline GLUINT GetID() const { return id; }

		// Create a render buffer with format, width and height.
		static GLRenderBuffer* Create(EGLTexture format, int width, int height);

		// Bind the render buffer.
		void Bind();
		
		// Unbind currently bounded render buffer.
		void Unbind();

		// Update RenderBuffer storage.
		void UpdateStorage(EGLTexture format, int width, int height);

	private:
		// OpenGL Object ID.
		GLUINT id;

		// RenderBuffer Storage Format.
		EGLFormat format;

		// RenderBuffer Storage width.
		int width;

		// RenderBuffer Storage height.
		int height;
	};

}


