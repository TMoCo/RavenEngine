#pragma once





#include "IRenderResource.h"

#include <memory>




namespace Raven
{
	class RenderRscShader;

	//
	//
	class RenderRscMaterial : public IRenderResource
	{
	public:
		// Construct.
		RenderRscMaterial(RenderRscShader* shader);

		// Destruct.
		~RenderRscMaterial();

		//
		inline RenderRscShader* GetShaderRsc() { return shaderRsc; }

	private:
		// The shader resource this material is.
		RenderRscShader* shaderRsc;
	};

}


