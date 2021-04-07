#pragma once



#include "RenderPrimitive.h"

#include "glm/vec4.hpp"



namespace Raven
{
	class RenderRscTerrain;


	// RenderTerrain:
	//		- Represent a Terrain while rendering.
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

	private:
		// Terrain Render Resrouce.
		RenderRscTerrain* terrainRsc;

	};


}

