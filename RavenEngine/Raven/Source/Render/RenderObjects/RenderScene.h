#pragma once




#include "RenderBatch.h"

#include "glm/matrix.hpp"





namespace Raven
{
	class RenderPrimitive;
	class RenderTerrain;



	// Render Scene Batches.
	enum class ERSceneBatch : uint32_t
	{
		Opaque = 0,
		Debug = 1,
		Translucent = 2,
	};




	// RenderScene:
	//		- The render representation of the scene.
	//
	class RenderScene
	{
	public:
		// Construct. 
		RenderScene();

		// Destruct. 
		~RenderScene();

		//
		void Build(RenderTerrain* terrain);

		// Set scene view.
		void SetView(const glm::mat4& mtx);

		// Set scene projection.
		void SetProjection(const glm::mat4& mtx);

		// Draw a specific batch.
		void Draw(ERSceneBatch type);

		// Add primitives to the debug batch to be draw by this scene.
		void AddDebugPrimitives(const std::vector<RenderPrimitive*>& primitives);
		
		// Clear all render data of the previous frame.
		void Clear();

	private:
		//
		void TraverseScene();

		// Return batch of a batchtype.
		inline RenderBatch& GetBatch(ERSceneBatch batchType) { return batches[(uint32_t)batchType]; }

	private:
		// Render Scene Batches.
		RenderBatch batches[3];

		// Main View
		glm::mat4 view;

		// Main Project.
		glm::mat4 projection;
		
	};


}


