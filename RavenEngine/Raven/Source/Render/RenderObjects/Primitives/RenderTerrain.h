#pragma once



#include "RenderPrimitive.h"

#include "glm/vec4.hpp"



namespace Raven
{
	class RenderRscTerrain;



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
		virtual void Draw(GLShader* shader) const override;

		// Return Expected Domain of this primitive.
		inline virtual ERenderShaderDomain GetDomain() { return ERenderShaderDomain::Terrain; }

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


}

