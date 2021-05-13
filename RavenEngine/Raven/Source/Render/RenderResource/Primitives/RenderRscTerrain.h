#pragma once


#include "RenderRscPrimitive.h"
#include "Render/RenderResource/RenderRscTexture.h"
#include "Math/BoundingBox.h"


#include "glm/vec2.hpp"



namespace Raven
{
	class GLVertexArray;
	class GLBuffer;
	class GLTexture;
	class UniformBuffer;
	class DynamicTexture;



	// a single bin in the terrain.
	struct TerrainBin
	{
		// Bin position offset in world space.
		glm::vec2 offset;

		// Texture Coordinate Scaling for each bin.
		glm::vec2 uvScale;

		// Bin Bounding Box, you can ignore the Y-Axis we only care about 2D-Bounds.
		MathUtils::BoundingBox bounds;
	};



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

		// Load Terrain Render Data:
		// @param inHeightMap: a Height map texture.
		// @param res: the resolution of the terrain mesh.
		// @param inScale: the scale in meter for the terrain mesh.
		// @param inHeight: the min/max height of the terrain mesh.
		// @param inNumBins: num of bins in a single row of the terrain.
		void Load(DynamicTexture* inHeightMap, int32_t inNumBins,
			const glm::ivec2& inScale, const glm::vec2& inHeight);

		// Set bins data to be referenced by the terrain render resrouce.
		inline void SetBins(const std::vector<TerrainBin>* inBins) { bins = inBins; }

		// Return terrain bins.
		inline const std::vector<TerrainBin>* GetBins() const { return bins; }

		// Return mesh vertex array.
		inline const GLVertexArray* GetArray() const { return vertexArray; }

		// Return the number of vertices in the terrain mesh.
		inline uint32_t GetNumVerts() const { return numVerts; }
		inline uint32_t GetNumIndices() const { return numIndices; }

		// Return the terrain sacle.
		inline const glm::vec2& GetScale() const { return scale; }

		// Return the height factor of the terrain.
		inline const glm::vec2& GetHeight() const { return height; }

		// Return the height map texture.
		inline DynamicTexture* GetHeightMap() const { return heightMapTexture; }

		// Bind the bin uniform buffer.
		inline UniformBuffer* GetBinUB() { return binUniform.get(); }
		 
	private:
		// Generate Terrain Mesh for a single ben.
		void GenerateTerrainMesh();

	private:
		// Height Map.
		DynamicTexture* heightMapTexture;

		// OpenGL Buffer for Terrain Mesh Positions.
		GLBuffer* positionBuffer;

		//
		GLBuffer* indexBuffer;

		/** The OpenGL Vertex Array of the terrain mesh, defines mesh vertex input. */
		GLVertexArray* vertexArray;

		// Number of vertices in the terrain mesh.
		uint32_t numVerts;

		// Number of indicess in the terrain mesh.
		uint32_t numIndices;

		// Terrain Plane(X,Y) Scale.
		glm::vec2 scale;

		// Terrain Height Scale.
		glm::vec2 height;

		// Number of bins in single row of the terrain.
		int32_t numBins;

		// The scale of a single bin.
		glm::vec2 binScale;

		// Terrain Bin Data Uniform Buffers.
		Ptr<UniformBuffer> binUniform;

		// Terrains bins.
		const std::vector<TerrainBin>* bins;
	};


}


