#pragma once




#include "GLTypes.h"
#include "glm/vec2.hpp"





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
		~GLRenderBuffer();

		// Return the opengl id of the buffer.
		inline GLUINT GetID() const { return id; }

		// Create a render buffer with format, width and height.
		static GLRenderBuffer* Create(EGLFormat format, int width, int height);

		// Bind the render buffer.
		void Bind();
		
		// Unbind currently bounded render buffer.
		void Unbind();

		// Update RenderBuffer storage with new size.
		void UpdateStorage(int width, int height);

		// Return current render buffer size.
		inline const glm::ivec2& GetSize() const { return size; }

	private:
		// OpenGL Object ID.
		GLUINT id;

		// RenderBuffer Storage Format.
		EGLFormat format;

		// RenderBuffer Storage size.
		glm::ivec2 size;

	};

}


