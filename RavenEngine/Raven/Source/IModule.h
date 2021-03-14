#pragma once



#include <vector>





/** Modules Types. */
enum EModuleType
{
	MT_Render,
	MT_ImGui,
	MT_Window,
	MT_GUI,
	MT_SceneManager,
	MT_MAX
};




/**
 * Interface for modules in the engine.
 */
class IModule
{
public:
	/** Virtual Destruct. */
	virtual ~IModule() { }

	/** Called by the engine to initialize the module. */
	virtual void Initialize() = 0;

	/** Called by the engine to destory the module. */
	virtual void Destroy() = 0;
};

