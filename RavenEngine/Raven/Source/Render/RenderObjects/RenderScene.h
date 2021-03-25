#pragma once




#include "RenderBatch.h"

#include "glm/matrix.hpp"





namespace Raven
{
	class RenderPrimitive;
	class RenderTerrain;
	class Scene;

	class GLBuffer;





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
		void Setup();

		// Build Render scene data form a scene.
		void Build(Scene* scene);

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

		// Return true if the batch is empty.
		inline bool IsEmpty(ERSceneBatch batch) { return batches[(uint32_t)batch].IsEmpty(); }

	private:
		//
		void TraverseScene(Scene* scene);

		// Return batch of a batchtype.
		inline RenderBatch& GetBatch(ERSceneBatch batchType) { return batches[(uint32_t)batchType]; }

		// Create New Primitive to render.
		template<class PrimitiveType>
		PrimitiveType* NewPrimitive()
		{
			// TODO: Memeory management for dynamic primitives.
			PrimitiveType* prim = new PrimitiveType();
			dynamicPrimitive.push_back(prim);

			return prim;
		}


	private:
		// Render Scene Batches.
		RenderBatch batches[3];

		// Main View
		glm::mat4 view;

		// Main Project.
		glm::mat4 projection;
		
		// Dynamic Primitives Container.
		std::vector<RenderPrimitive*> dynamicPrimitive;

		// ~MinimalSolution --- ---- --- ---- --- ---- ---
		GLBuffer* trUBO;
		GLBuffer* lightingUBO;
		GLBuffer* materialUBO;
		// ~MinimalSolution --- ---- --- ---- --- ---- ---

		// If primitives does not have materials, we use this one.
		class RenderRscShader* defaultShader;
		class RenderRscMaterial* defaultMaterail;
	};


}


