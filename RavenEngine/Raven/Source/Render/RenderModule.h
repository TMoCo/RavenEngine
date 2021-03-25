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
	class RenderTarget;
	class Camera;
	class Transform;
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
		RenderTarget* GetSceneRT() { return rtScene.get(); }

		// if true will resize the render target with engine window.
		inline void SetRTToWindow(bool value) { isRTToWindow = value; }

	public:
		// Update render.
		void Update(float dt);

		// Beging and Prepare the render
		void BeginRender(Scene* scene, const glm::ivec2& extent);

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

		// The Render Target we draw our scene on.
		std::shared_ptr<RenderTarget> rtScene;

		// if true will render to window with the exact size as the window.
		bool isRTToWindow;


	};

}


