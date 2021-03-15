#pragma once




#include "Utilities/Core.h"
#include "IModule.h"
#include "RenderSurface.h"

#include "glm/vec2.hpp"


#include <memory>





namespace Raven
{
	class GLContext;
	class RenderScene;
	class RenderDebug;
	class Scene;
	class GLTexture;


	// RenderModule:
	//		- The Engine Render Manager.
	//
	class RenderModule : public IModule
	{
	public:
		// Construct.
		RenderModule();

		// Destruct.
		~RenderModule();

		// Return the type of the module.
		static EModuleType GetModuleType() { return MT_Render; }

		// Return the required render surface by the renderer.
		static RenderSurface GetRequiredRenderSurface();

		// Return render debug.
		inline RenderDebug* GetDebug() { return rdebug.get(); }

		// Return the final render texture target of the scene.
		GLTexture* GetSceneRT();

	public:
		// Update render.
		void Update(float dt);

		// Beging and Prepare the render
		void BeginRender(Scene* scene, bool blit, const glm::ivec2& extent);

		// Render
		void Render();

		// End and clean up the render.
		void EndRender();

	private:
		// Module Initialize.
		virtual void Initialize() override;

		// Module Destroy.
		virtual void Destroy() override;

	private:
		// The context of the render.
		std::unique_ptr<GLContext> context;

		// if true the engine currently rendering.
		bool isRendering;

		// 
		std::shared_ptr<RenderScene> rscene;

		// Render Debug, used for debug drawing.
		std::unique_ptr<RenderDebug> rdebug;
	};

}


