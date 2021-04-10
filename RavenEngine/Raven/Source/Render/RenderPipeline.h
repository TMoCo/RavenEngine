#pragma once




#include "Utilities/Core.h"


#include "glm/vec2.hpp"
#include "glm/vec4.hpp"




namespace Raven
{
	class RenderTarget;
	class RenderScene;
	class RenderPass;
	class RenderScreen;
	class RenderRscShader;
	class UniformBuffer;


	// Uniforms of the render pipline.
	struct RenderPipelineUniforms
	{
		// Common Uniform Buffer - used by all shaders.
		Ptr<UniformBuffer> common;

		//
		Ptr<UniformBuffer> light;
	};



	// RenderPipeline:
	//		- The Engine Render Pipeline, Manage all render passes that is used to render our 
	//			scenes from start to finish.
	//
	//		- Only one instance created/managed by the RenderModule.
	//
	class RenderPipeline
	{
		// Friend...
		friend class RenderModule;

		// Private Construct, @see RenderModule.
		RenderPipeline();

	public:
		// Destruct.
		~RenderPipeline();

		// Initializie the pipeline.
		void Initialize();

		// Destroy the pipeline.
		void Destroy();

		// Begin Rendering The Pipeline.
		void Begin(RenderTarget* target, RenderScene* scene, float time);

		// End Rendering The Pipeline.
		void End();

		// Render the scene into the target using the pipeline.
		void Render();

	private:
		// Build the render passes of the render pipeline.
		void SetupRenderPasses();

		// Load & Build the shaders of the render pipeline.
		void SetupShaders();

		// Resize all render passes.
		void Resize(const glm::ivec2& newSize);

	private:
		// Deferred Render Pass - G-Buffer.
		Ptr<RenderPass> gbuffer;

		// Deferred Render Pass - Lighting.
		Ptr<RenderPass> lighting;

		// Forward Rendering Pass - used mostly for translucent.
		Ptr<RenderPass> forward;

		// Lighting shader for the lighting render pass.
		Ptr<RenderRscShader> lightingShader;

		// Screen triangle used to render the entire screen, used by render passes and post-processing.
		Ptr<RenderScreen> rscreen;

		// Uniforms of the pipeline.
		RenderPipelineUniforms uniforms;

		// The current render target of the scene we are rendering on.
		RenderTarget* rtarget;

		// The current scene we are rendering.
		RenderScene* rscene;

		// The viewport used for rendering.
		glm::vec4 viewport;
	};


}