#pragma once




#include "GLTypes.h"





namespace Raven
{



	//	OpenGL buffer.
	//
	class GLBuffer
	{
	private:
		// Privte construct, use GLBuffer::Create to create buffer objects.
		GLBuffer();

	public:
		// Destruct.
		~GLBuffer();

		// Create a new GLBuffer and allocated data of size. 
		static GLBuffer* Create(EGLBufferType type, int size, EGLBufferUsage usage);

		// Create a new GLBuffer with data. 
		static GLBuffer* Create(EGLBufferType type, int size, void* data, EGLBufferUsage usage);

		// Reallocate and update new data for the buffer.
		void UpdateData(int dataSize, void* data);

		// Update buffer with a new data.
		void UpdateSubData(int dataSize, int offset, void* data);

		// Return the current size of the buffer.
		inline int GetSize() { return size; }

		// Return the current type of the buffer.
		inline EGLBufferType GetType() { return type; }

		// Return the current usage of the buffer.
		inline EGLBufferUsage GetUsage() { return usage; }

		// Return the opengl id of the buffer.
		inline GLUINT GetID() const { return id; }

		// Bind the buffer to its target type.
		void Bind();

		// Unbind all buffers from target type.
		void Unbind();

		// Bind a buffer to a specific binding index, used by Unifrom Buffers to bind it to.
		void BindBase(int binding);

		// Bind a buffer to a specific binding index, used by Unifrom Buffers to bind it to.
		void BindRange(int binding, int offset, int size);

	private:
		// The type of the buffer.
		EGLBufferType type;

		// The usage of the buffer.
		EGLBufferUsage usage;

		// The current size of the buffer.
		int size;

		// OpenGL Object ID.
		GLUINT id;
	};


}




