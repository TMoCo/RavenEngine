#pragma once



#include "RenderPrimitive.h"

#include "glm/vec4.hpp"



namespace Raven
{
	class RenderRscTerrain;


	// RenderTerrain:
	//		-
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

		// Set color.
		inline void SetHeight(const glm::vec4& value) { color = value; }

		// Return color.
		inline const glm::vec4& GetHeight() { return color; }

		//
		void SetTerrainRsc(RenderRscTerrain* terrain);

		//
		void SetHeight(float value);

		//
		virtual void Draw(GLShader* shader) const override;

	private:
		// The Color.
		glm::vec4 color;

		//
		RenderRscTerrain* terrainRsc;

		//
		float height;
	};


}

