#pragma once







namespace Raven
{

	// IRenderResource:
	//      - Every resource that has its own render data, its render data is managed by this resource. 
	//
	//      - Example of a render resource is RenderTextureResource which holds the allocated texture on 
	//        the GPU. The render then can use that render resource to reference it when drawing.
	//
	class IRenderResource
	{
	public:
		// Virtual Destructor.
		virtual ~IRenderResource() { }

	};


}


