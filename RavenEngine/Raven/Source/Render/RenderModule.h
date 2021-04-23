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
	class RenderTarget;
	class Scene;
	class RenderPipeline;
	class RenderTexFilter;
	class RenderRscMaterial;
	class Material;




	// The Renderer Default Materials.
	struct RenderDefaultMaterials
	{
		// The Default Material for meshes.
		Ptr<Material> mesh;

		// The Default Material for skinned meshes.
		Ptr<Material> skinned;

		// The Default Material for terrain.
		Ptr<Material> terrain;
	};






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

		//  Return default materails.
		inline const RenderDefaultMaterials& GetDefaultMaterials() { return defaultMaterials; }

	public:
		// Update render.
		void Update(float dt);

		// Beging and Prepare the render
		void BeginRender(Scene* scene, const glm::ivec2& extent);

		// Render
		void Render();

		// End and clean up the render.
		void EndRender();

		// Create/Setup default materials.
		void CreateDefaultMaterials();

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

		// The Main Render data of the scene. 
		Ptr<RenderScene> rscene;

		// The Main Render Target of the scene. 
		Ptr<RenderTarget> rtScene;

		// Render Debug, used for debug drawing.
		Ptr<RenderDebug> rdebug;

		// The Engine Render Pipeline.
		Ptr<RenderPipeline> rpipeline;

		// The Engine Render Texture Filter, used to filter textures.
		Ptr<RenderTexFilter> rfilter;

		// if true will render to window with the exact size as the window.
		bool isRTToWindow;

		// The default materials.
		RenderDefaultMaterials defaultMaterials;
	};

}


