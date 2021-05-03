#pragma once





#include "RenderRscPrimitive.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include <vector>





namespace Raven
{
	class GLVertexArray;
	class GLBuffer;

	// RenderRscMesh:
	//
	class RenderRscMesh : public RenderRscPrimitive
	{
	public:
		// Construct.
		RenderRscMesh();

		// Destruct.
		~RenderRscMesh();

		// Load Mesh Data
		virtual void Load(const std::vector<glm::vec3>& positions,
			const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& tangents,
			const std::vector<glm::vec2>& texCoord, const std::vector<unsigned int>& indices);

		// Return the vertex array that defines this mesh Vertex Input.
		inline GLVertexArray* GetArray() { return vxarray; }

		// Return the number of indices in the mesh.
		inline int32_t GetNumIndices() const { return numIndices; }


	protected:
		// The OpenGL Vertex Array of the mesh, defines mesh vertex input.
		GLVertexArray* vxarray;

		// OpenGL Buffer for Mesh Positions.
		GLBuffer* positionBuffer;

		// OpenGL Buffer for Mesh Normals.
		GLBuffer* normalBuffer;

		// OpenGL Buffer for Mesh Normals.
		GLBuffer* tangentBuffer;

		// OpenGL Buffer for Mesh Texture Coordinate.
		GLBuffer* texCoordBuffer;

		// OpenGL Buffer for Mesh Indices.
		GLBuffer* indexBuffer;

		// Number of indices in the index buffer.
		int32_t numIndices;
	};



	// RenderRscSkinnedMesh:
	//
	class RenderRscSkinnedMesh : public RenderRscPrimitive
	{
	public:
		// Construct.
		RenderRscSkinnedMesh();

		// Destruct.
		~RenderRscSkinnedMesh();

		// Load Mesh Data
		void Load(const std::vector<glm::vec3>& positions,
			const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& tangents,
			const std::vector<glm::vec2>& texCoord, const std::vector<unsigned int>& indices,
			const std::vector<glm::vec4>& weight, const std::vector<glm::ivec4>& blendIndices);

		// Return the vertex array that defines this mesh Vertex Input.
		inline GLVertexArray* GetArray() { return vxarray; }

		// Return the number of indices in the mesh.
		inline int32_t GetNumIndices() const { return numIndices; }

	private:
		// The OpenGL Vertex Array of the mesh, defines mesh vertex input.
		GLVertexArray* vxarray;

		// OpenGL Buffer for Mesh Positions.
		GLBuffer* positionBuffer;

		// OpenGL Buffer for Mesh Normals.
		GLBuffer* normalBuffer;

		// OpenGL Buffer for Mesh Normals.
		GLBuffer* tangentBuffer;

		// OpenGL Buffer for Mesh Texture Coordinate.
		GLBuffer* texCoordBuffer;

		// OpenGL Buffer for Mesh Indices.
		GLBuffer* indexBuffer;

		// Number of indices in the index buffer.
		int32_t numIndices;

		// OpenGL Buffer for skin weight.
		GLBuffer* weightBuffer;

		// OpenGL Buffer for bone indices.
		GLBuffer* boneIndicesBuffer;
	};

}


