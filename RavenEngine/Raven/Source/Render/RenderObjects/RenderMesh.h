#pragma once



#include "RenderPrimitive.h"




namespace Raven
{
	class RenderRscMesh;
	

	//
	//
	class RenderMesh : public RenderPrimitive
	{	
	public:
		//
		RenderMesh();

		//
		~RenderMesh();

	private:
		// The Mesh Render Resource.
		RenderRscMesh* mesh;
	};

}
