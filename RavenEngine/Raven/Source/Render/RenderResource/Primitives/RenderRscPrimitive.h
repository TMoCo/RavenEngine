#pragma once


#include "Render/RenderResource/IRenderResource.h"



namespace Raven
{

	// RenderRscPrimitive:
	//		- Parent class for all resources that can be drawn.
	//		
	class RenderRscPrimitive : public IRenderResource
	{
	public:
		// Destruct. 
		virtual ~RenderRscPrimitive() { }

	};


}


