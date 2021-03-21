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

		// Set the mesh to be drawn.
		void SetMesh(RenderRscMesh* inMesh);

		// Draw Mesh.
		void Draw(GLShader* shader) const override;

	private:
		// The Mesh Render Resource.
		RenderRscMesh* mesh;
	};

}
