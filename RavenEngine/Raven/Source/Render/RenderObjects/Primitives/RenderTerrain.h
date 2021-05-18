#pragma once



#include "RenderPrimitive.h"

#include "glm/vec4.hpp"



namespace Raven
{
	class RenderRscTerrain;
	class RenderRscMeshInstance;




	// RenderTerrain:
	//		- Represent a Terrain while rendering, draw a single terrain bin.
	//
	class RenderTerrain : public RenderPrimitive
	{
	public:
		// Construct. 
		RenderTerrain();

		// Destruct. 
		virtual ~RenderTerrain();

		// Return the primitive resource.
		virtual RenderRscPrimitive* GetRsc() override;

		// Set the terrain render resource to render.
		void SetTerrainRsc(RenderRscTerrain* terrain);

		// Draw the terrain.
		virtual void Draw(GLShader* shader, bool isShadow) const override;

		// Return Expected Domain of this primitive.
		inline virtual ERenderShaderDomain GetDomain() override { return ERenderShaderDomain::Terrain; }

		// Add a new bin to be drawn.
		inline void SetBin(uint32_t index) { binIndex = index; }

		// Retrun the bin this RenderPrimitive draw.
		inline uint32_t GetBin(uint32_t index) const { return binIndex; }

	private:
		// Terrain Render Resrouce.
		RenderRscTerrain* terrainRsc;

		// Bins to be drawn.
		uint32_t binIndex;
	};



	// RenderTerrainFoliage:
	//    - render a single terrain mesh section of a foliage layer.
	//
	class RenderTerrainFoliage : public RenderPrimitive
	{
	public:
		// Construct. 
		RenderTerrainFoliage();

		// Destruct. 
		virtual ~RenderTerrainFoliage();

		// Return the primitive resource.
		virtual RenderRscPrimitive* GetRsc() override;

		// Set the terrain render resource to render.
		void SetMeshRsc(RenderRscMeshInstance* rsc);

		// Number of instance meshes to draw.
		void SetInstanceCount(int32_t inCount);

		// Draw the mesh instances.
		virtual void Draw(GLShader* shader, bool isShadow) const override;

		// Return Expected Domain of this primitive.
		inline virtual ERenderShaderDomain GetDomain() override { return ERenderShaderDomain::MeshInstance; }

	private:
		// Mesh Instance Resrouce.
		RenderRscMeshInstance* instanceRsc;

		// Number of instance to draw.
		int32_t count;
	};


}

