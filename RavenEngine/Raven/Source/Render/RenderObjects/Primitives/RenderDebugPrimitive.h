#pragma once



#include "RenderPrimitive.h"

#include "glm/vec4.hpp"



namespace Raven
{
	class RenderRscPrimitive;
	class RenderRscMaterial;
	class RenderRscDebugMesh;



	// RenderDebugPrimitive:
	//		- Render primitive for debug derawing.
	//		
	class RenderDebugPrimitive : public RenderPrimitive
	{
	public:
		// Construct. 
		RenderDebugPrimitive();

		// Destruct. 
		virtual ~RenderDebugPrimitive();

		// Return the primitive resource.
		virtual RenderRscPrimitive* GetRsc() override;

		// Set color.
		inline void SetColor(const glm::vec4& value) { color = value; }

		// Return color.
		inline const glm::vec4& GetColor() { return color; }

		//
		void SetDebugMesh(RenderRscDebugMesh* debugMesh);

		//
		virtual void Draw(GLShader* shader) const override;

		// Return Expected Domain of this primitive.
		inline virtual ERenderShaderDomain GetDomain() { return ERenderShaderDomain::Custom; }

	private:
		// The Color.
		glm::vec4 color;

		//
		RenderRscDebugMesh* mesh;
	};


}


