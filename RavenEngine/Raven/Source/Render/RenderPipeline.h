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
	class RenderSphere;
	class RenderGrid;
	class RenderRscShader;
	class UniformBuffer;
	class GLTexture;
	class GLShader;



	// Uniforms of the render pipline.
	struct RenderPipelineUniforms
	{
		// Common Uniform Buffer - used by all shaders.
		Ptr<UniformBuffer> common;

		// Light framebuffer used for deferred lighting data.
		Ptr<UniformBuffer> light_DEFERRED;

		// Light framebuffer used for forward lighting data.
		Ptr<UniformBuffer> light_FORWARD;

		// Shadow Uniform Buffer used while drawing the shadow maps.
		Ptr<UniformBuffer> shadow;

		// Shadow Uniform Buffer, used in the lighting pass.
		Ptr<UniformBuffer> lightShadow;
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
		RenderPipeline(Ptr<RenderScreen> screen, Ptr<RenderSphere> sphere);

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

		// Setup Screen Space Ambient Occlusion.
		void SetupSSAO();

		// Resize all render passes.
		void Resize(const glm::ivec2& newSize);

		// Update Light Unifrom Buffer for deferred lighting.
		void UpdateLights_DEFERRED();
		
		// Update Shadows Unifrom Buffer..
		void UpdateShadows();

		// Do a final post processing and use the final render target.
		void DoPostProcessFinal(int32_t hdrTargetIndex);

	private:
		// Deferred Render Pass - G-Buffer.
		Ptr<RenderPass> gbufferPass;

		// Deferred Render Pass - Lighting.
		Ptr<RenderPass> lightingPass;

		// Forward Rendering Pass - used mostly for translucent.
		Ptr<RenderPass> forwardPass;

		// The final post-processing step.
		Ptr<RenderPass> finalPostProcessPass;

		// The SSAO pass.
		Ptr<RenderPass> ssaoPass;
		Ptr<RenderPass> ssaoBlurPass;

		// Pipeline HDR Render Targets.
		Ptr<GLTexture> hdrTarget[2];

		// Lighting shader for the lighting render pass.
		Ptr<RenderRscShader> lightingShader;

		// Final post processing shader used to do Gamma correct, tone mapping etc...
		Ptr<RenderRscShader> finalPostProcessShader;

		// Fast Approximate Anti-Aliasing executed on the final pos-tprocesing step
		Ptr<RenderRscShader> fxaaShader;

		// SSAO Shader.
		Ptr<RenderRscShader> ssaoShader;
		
		// SSAO Shader.
		Ptr<RenderRscShader> ssaoBlurShader;

		// Screen triangle used to render the entire screen, used by render passes and post-processing.
		Ptr<RenderScreen> rscreen;

		// Sphere.
		Ptr<RenderSphere> rsphere;

		// Uniforms of the pipeline.
		RenderPipelineUniforms uniforms;

		// The current render target of the scene we are rendering on.
		RenderTarget* rtarget;

		// The current scene we are rendering.
		RenderScene* rscene;

		// The viewport used for rendering.
		glm::vec4 viewport;

		// Render Pipeline targets size;
		glm::ivec2 size;

		// The Render Grid.
		Ptr<RenderGrid> rgrid;
		
		//
		Ptr<GLTexture> ssaoNoiseTexture;

	public:
		// ~ITERATION_0----------------------------------
		class GLTexture* testEnv;
		class GLTexture* testBRDF;
		// ~ITERATION_0----------------------------------

	};


}