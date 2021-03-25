#pragma once


#include "RenderRscPrimitive.h"
#include "RenderRscTexture.h"


#include "glm/vec2.hpp"



namespace Raven
{
	class GLVertexArray;
	class GLBuffer;
	class GLTexture;



	// RenderRscPrimitive:
	//		- Parent class for all resources that can be drawn.
	//		
	class RenderRscTerrain : public RenderRscPrimitive
	{
	public:
		// Construct.
		RenderRscTerrain();

		// Destruct..
		~RenderRscTerrain();

		// Load a grayscale height map image
		void LoadHeightMap(int width, int height, const void* data);

		// Generate Terrain Mesh
		void GenerateTerrain(int32_t inRes, const glm::vec2& inScale, float inHeight);

		// Return mesh vertex array.
		inline const GLVertexArray* GetArray() const { return vertexArray; }

		// Return the number of vertices in the terrain mesh.
		inline uint32_t GetNumVerts() const { return numVerts; }

		// Return the terrain sacle.
		inline glm::vec2 GetScale() const { return scale; }

		// Return the height map texture.
		inline GLTexture* GetHeightMap() const { return heightMap; }

		// Return the height factor of the terrain.
		inline float GetHeight() const { return height; }

	private:
		// Height Map.
		GLTexture* heightMap;

		/** The OpenGL Vertex Array of the terrain mesh, defines mesh vertex input. */
		GLVertexArray* vxarray;

		// OpenGL Buffer for Terrain Mesh Positions.
		GLBuffer* positionBuffer;

		// Vertex Array.
		GLVertexArray* vertexArray;

		// Number of vertices in the terrain mesh.
		uint32_t numVerts;

		// Terrain Plane(X,Y) Scale.
		glm::vec2 scale;

		// Terrain Height Scale.
		float height;
	};


}


