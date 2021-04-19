#pragma once



#include "RenderPrimitive.h"




namespace Raven
{
	class RenderRscSkinnedMesh;
	

	//	RenderMesh:
	//			- Represent a Mesh while rendering.
	class RenderSkinnedMesh : public RenderPrimitive
	{	
	public:
		// Construct.
		RenderSkinnedMesh();

		// Destruct.
		~RenderSkinnedMesh();

		// Return the primitive resource.
		virtual RenderRscPrimitive* GetRsc() override;

		// Set the mesh to be drawn.
		void SetMesh(RenderRscSkinnedMesh* inMesh);

		// Draw Mesh.
		void Draw(GLShader* shader) const override;

	private:
		// The Mesh Render Resource.
		RenderRscSkinnedMesh* mesh;
	};

}
