//

#pragma once


#include "IModule.h"


#include <array>





struct GLFWwindow;





/**
 * Engine:
 *		- 
 */
class Engine
{
private:
  /** Constructor. */
	Engine();

public:
	/** Destructor. */
	~Engine();

	/** Return the Engine Singleton. */
	static Engine& Get();

	/** Initialize the Engine. */
  void Initialize();

	/** Run the Engine. */
	int Run();


	/** Static function returns the Module of type Module Type. */
	template<class TModule>
	static inline TModule* GetModule()
	{
		return static_cast<TModule*>( Get().engineModules[TModule::GetModuleType() ] );
	}

private:
	/** Create a the module and store it in the engine modules list. */
	template<class TModule>
	inline void CreateModule()
	{
		engineModules[ TModule::GetModuleType() ] = new TModule();
	}

	/** Initialize a module. */
	template<class TModule>
	inline void InitializeModule()
	{
		engineModules[ TModule::GetModuleType() ]->Initialize();
	}

	/** Destroy a module. */
	template<class TModule>
	inline void DestroyModule()
	{
		engineModules[ TModule::GetModuleType() ]->Destroy();
	}

	/** Load required modules. */
	void LoadModules();

	/** Destory all loaded modules. */
	void DestoryModules();

private:
	/** The GLFW Window created. */
	GLFWwindow* glfw_window;

	/** List of all the modules in the engine. */
	std::array<IModule*, MT_MAX> engineModules;
};

