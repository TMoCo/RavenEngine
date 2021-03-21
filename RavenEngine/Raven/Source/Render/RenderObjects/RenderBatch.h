#pragma once





#include <vector>



namespace Raven
{
	class RenderPrimitive;



	// RenderBatch:
	//		- a batch of render primitives to be drawn.
	//
	class RenderBatch
	{
	public:
		// Construct. 
		RenderBatch();

		// Destruct. 
		~RenderBatch();

		// 
		void Add(RenderPrimitive* primitive);

		//
		void Clear();

		//
		std::vector<RenderPrimitive*> elements;
	};


}


