#pragma once




#include "Utilities/Core.h"
#include "RenderBatch.h"

#include "glm/matrix.hpp"





namespace Raven
{
	class RenderPrimitive;
	class RenderLight;
	class UniformBuffer;
	class Scene;





	// Render Scene Batches.
	enum class ERSceneBatch : uint32_t
	{
		Opaque = 0,
		Debug = 1,
		Translucent = 2,
	};


	// The Environment Properties of the scene.
	struct RenderSceneEnvironment
	{
		//
		glm::vec3 sunDir;

		//
		glm::vec3 sunColor;

		//
		float sunPower;

		// Reset All Environment Properties.
		void Reset();
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
		void SetProjection(const glm::mat4& mtx, float n, float f);

		// Draw a specific batch.
		void Draw(ERSceneBatch type);

		// Add primitives to the debug batch to be draw by this scene.
		void AddDebugPrimitives(const std::vector<RenderPrimitive*>& primitives);
		
		// Clear all render data of the previous frame.
		void Clear();

		// Return true if the batch is empty.
		inline bool IsEmpty(ERSceneBatch batch) { return batches[(uint32_t)batch].IsEmpty(); }

		// Return the view projection matrix.
		inline const glm::mat4& GetViewProjection() const { return viewProjMatrix; }

		// Return the inverse of the view projection matrix.
		inline const glm::mat4& GetViewProjectionInverse() const { return viewProjMatrixInverse; }

		// Return the view direction.
		inline const glm::vec3& GetViewDir() const { return viewDir; }

		// Return the view position.
		inline const glm::vec3& GetViewPos() const { return viewPos; }

		// Return the scene enviornment.
		inline const RenderSceneEnvironment& GetEnvironment() const { return environment; }

		// Return near clipping plane.
		inline const float& GetNear() const { return near; }

		// Return far clipping plane.
		inline const float& GetFar() const { return far; }

		// Return the light in the scene.
		inline const std::vector<RenderLight*>& GetLights() { return lights; }

	private:
		// Collect view & projection from the scene.
		void CollectSceneView(Scene* scene);

		// Collect lights from the scene.
		void CollectSceneLights(Scene* scene);

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

		// Create New Primitive to render.
		template<class LightType>
		inline LightType* NewLight()
		{
			// TODO: Memeory management for lights.
			LightType* light = new LightType();
			lights.push_back(light);

			return light;
		}


	private:
		// The main render batches of the scene.
		RenderBatch batches[3];

		// The View Matrix.
		glm::mat4 view;

		// Main Project.
		glm::mat4 projection;

		// View & Projection Matrix
		glm::mat4 viewProjMatrix;

		// View & Projection Matrix Inverse
		glm::mat4 viewProjMatrixInverse;

		// The View Direction.
		glm::vec3 viewDir;

		// The View Position.
		glm::vec3 viewPos;

		// Near Clipping Plane.
		float near;

		// Far Clipping Plane.
		float far;

		// Scene Enviornment Data.
		RenderSceneEnvironment environment;

		// Dynamic Primitives Container.
		std::vector<RenderPrimitive*> dynamicPrimitive;

		// Transform Uniform Buffer.
		Ptr<UniformBuffer> transformUB;

		// Lights in the scene.
		std::vector<RenderLight*> lights;
	};


}


