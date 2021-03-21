#pragma once





#include "RenderRscPrimitive.h"

#include "glm/vec3.hpp"
#include <vector>





namespace Raven
{
	class GLVertexArray;
	class GLBuffer;



	//
	//
	class RenderRscDebugMesh : public RenderRscPrimitive
	{
	public:
		// Construct.
		RenderRscDebugMesh();

		// Destruct.
		~RenderRscDebugMesh();

		// Create a debug box mesh.
		void CreateBox();

		// Return mesh vertex array.
		inline const GLVertexArray* GetArray() const { return vertexArray; }

		//
		inline uint32_t GetNumIndices() const { return numIndices; }

	private:
		// Load a mesh data into the render resoruce.
		void Load(const std::vector<glm::vec3>& verts, const std::vector<uint32_t>& indices);

	private:
		// Mesh Vertex Array.
		GLVertexArray* vertexArray;

		// Mesh Vertex Buffer.
		GLBuffer* vertexBuffer;

		// Mesh Index Buffer.
		GLBuffer* indexBuffer;

		//
		uint32_t numIndices;
	};

}


