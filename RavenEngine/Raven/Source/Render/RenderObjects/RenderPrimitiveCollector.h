#pragma once



#include "Utilities/Core.h"


#include "glm/matrix.hpp"
#include <vector>



namespace Raven
{
	class RenderScene;
	class RenderPrimitive;
	class RenderMesh;
	class RenderSkinnedMesh;


	// RenderPrimitiveCollector:
	//    - Used to collect render primitives.
	//
	class RenderPrimitiveCollector
	{
		// Friend...
		friend class RenderScene;

	public:
		// Construct.
		RenderPrimitiveCollector(RenderScene* sceneOwner);

		// Create new render primitive for collection.
		RenderMesh* NewMesh();

		// Create new render primitive for collection.
		RenderSkinnedMesh* NewSkinnedMesh();

	private:
		// Clear all collected primitives form the collector.
		void Reset();

	private:
		// The Render Scene.
		RenderScene* owner;

		// The new primitives collected.
		std::vector<RenderPrimitive*> primitive;

		// Current World Matrix to set the new render primitives 
		glm::mat4 worldMatrix;

		// Current Normal Matrix to set the new render primitives 
		glm::mat4 normalMatrix;
	};

}


