#pragma once



#include "RenderPrimitive.h"




namespace Raven
{
	class RenderRscMesh;
	

	//	RenderMesh:
	//			- Represent a Mesh while rendering.
	class RenderMesh : public RenderPrimitive
	{	
	public:
		// Construct.
		RenderMesh();

		// Destruct.
		~RenderMesh();

		// Return the primitive resource.
		virtual RenderRscPrimitive* GetRsc() override;

		// Set the mesh to be drawn.
		void SetMesh(RenderRscMesh* inMesh);

		// Draw Mesh.
		void Draw(GLShader* shader, bool isShadow) const override;

		// Return Expected Domain of this primitive.
		inline virtual ERenderShaderDomain GetDomain() { return ERenderShaderDomain::Mesh; }

	private:
		// The Mesh Render Resource.
		RenderRscMesh* mesh;
	};

}
