#pragma once



#include "RenderPrimitive.h"

#include "glm/matrix.hpp"

#include <vector>



namespace Raven
{
	class RenderRscPrimitive;
	class RenderRscMaterial;
	class GLShader;



	// RenderPrimitive:
	//		- Primitive dynamic render data used by the render to draw a single primitive.
	//		
	class RenderPrimitive
	{
	public:
		// Construct. 
		RenderPrimitive();

		// Destruct. 
		virtual ~RenderPrimitive();

		// Return the primitive resource.
		virtual RenderRscPrimitive* GetRsc() = 0;

		// Return material.
		inline RenderRscMaterial* GetMaterial() { return material; }

		// Set Render Resrouce Material to be used by for drawing this primitive.
		void SetMaterial(RenderRscMaterial* mat);

		// Set the world transformation.
		void SetWorldMatrix(const glm::mat4& mtx);

		// Set the normals world transformation.
		void SetNormalMatrix(const glm::mat4& mtx);

		// Return world transformation.
		inline const glm::mat4& GetWorldMatrix() const { return worldMatrix; }

		// Draw ths Primitive.
		virtual void Draw(GLShader* shader) const = 0;

	private:
		// The material.
		RenderRscMaterial* material;

		// Primitive World Transform.
		glm::mat4 worldMatrix;

		// Primitive Normal Transform.
		glm::mat4 normalMatrix;

	};


}


