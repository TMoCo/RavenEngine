#pragma once





#include "RenderRscPrimitive.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <vector>





namespace Raven
{
	class GLVertexArray;
	class GLBuffer;



	//
	//
	class RenderRscMesh : public RenderRscPrimitive
	{
	public:
		// Construct.
		RenderRscMesh();

		// Destruct.
		~RenderRscMesh();

		// Load Mesh Data
		void Load(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals,
			const std::vector<glm::vec2>& texCoord, const std::vector<unsigned int>& indices);

		// Return the vertex array that defines this mesh Vertex Input.
		inline GLVertexArray* GetArray() { return vxarray; }

		// Return the number of indices in the mesh.
		inline int32_t GetNumIndices() const { return numIndices; }


	private:
		/** The OpenGL Vertex Array of the mesh, defines mesh vertex input. */
		GLVertexArray* vxarray;

		// OpeGL Buffer for Mesh Positions.
		GLBuffer* positionBuffer;

		// OpeGL Buffer for Mesh Normals.
		GLBuffer* normalBuffer;

		// OpeGL Buffer for Mesh Texture Coordinate.
		GLBuffer* texCoordBuffer;

		// OpeGL Buffer for Mesh Indices.
		GLBuffer* indexBuffer;

		// Number of indices in the index buffer.
		int32_t numIndices;
	};

}


