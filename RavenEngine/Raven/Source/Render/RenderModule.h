#pragma once




#include "IModule.h"
#include <memory>





namespace Raven
{
	class GLContext;



	//
	//
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

	public:
		// Beging and Prepare the render
		void BeginRender();

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

		//
		bool isRendering;
	};

}


