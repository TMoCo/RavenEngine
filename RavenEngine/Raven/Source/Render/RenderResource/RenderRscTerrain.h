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

		//
		void GenerateTerrain(int resolution, const glm::vec2& scale);

		// Return mesh vertex array.
		inline const GLVertexArray* GetArray() const { return vertexArray; }

		//
		inline uint32_t GetNumVerts() const { return numVerts; }

		//
		inline glm::vec2 GetScale() const { return terrainScale; }

		//
		inline GLTexture* GetHeightMap() const { return heightMap; }

	private:
		// Height Map.
		GLTexture* heightMap;

		//
		GLVertexArray* vertexArray;

		//
		GLBuffer* vertexBuffer;

		//
		uint32_t numVerts;

		//
		glm::vec2 terrainScale;
	};


}


