#pragma once


#include "GLTypes.h"
#include "GLBuffer.h"

#include <vector>




namespace Raven
{
	// Data used by vertex array to build attribute bindings.
	class GLVABuildAttribData
	{
		friend class GLVertexArray;

	public:
		// Construct.
		GLVABuildAttribData(
			const GLBuffer* inBuffer,
			GLUINT inIndex, int inSize, EGLTypes inType,
			int inStride, int inOffset, int inInstance = 0 )
			: buffer(inBuffer)
			, index(inIndex)
			, size(inSize)
			, type(inType)
			, stride(inStride)
			, offset(inOffset)
			, instance(inInstance)
		{

		}

	private:
		// The buffer the attribute is bounded to.
		const GLBuffer* buffer;

		// Attribute Index.
		GLUINT index;

		// Attribute Data Type Size.
		int size;

		// Attribute Data Type.
		EGLTypes type;

		// Attribute Stride in the buffer.
		int stride;

		// Attribute Offset .
		int offset;

		// Instancing 
		int instance;
	};




	//
	// OpenGL Vertex Array.
	//
	class GLVertexArray
	{
	private:
		// Constrcut. 
		GLVertexArray();

	public:
		// Destruct.
		~GLVertexArray();

		// Create a vertex array.
		static GLVertexArray* Create();

		// Build a Vertex Array by binding vertex buffer to vertex shader input/attributes.
		// @param data: attibute description data used for binding.
		// @param elementBuffer: optional element buffer to bind, pass null if you don't want element buffer.
		void Build(const std::vector<GLVABuildAttribData>& data, const GLBuffer* elementBuffer);

		// Bind this vertex array.
		void Bind() const;

		// Unbind any currently bounded vertex array.
		void Unbind() const;

		// Return the opengl id.
		inline GLUINT GetID() const { return id; }

	private:
		// OpenGL object id.
		GLUINT id;
	};


}
