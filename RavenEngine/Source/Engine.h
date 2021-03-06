//



#pragma once

#include "IModule.h"


#include <array>


struct GLFWwindow;






/**
 *
 */
class Engine
{
private:
  /** */
	Engine();

public:
	/** */
	~Engine();

	/** */
	static Engine& Get();

	/** */
	static void Initialize();

	/** */
	static int Run();

	/** */
	template<class ModuleType>
	static inline ModuleType* GetModule()
	{
		return static_cast<ModuleType*>( modules[ ModuleType::GetType() ] );
	}


private:
	/** */
	static void LoadModules();



private:
	static GLFWwindow* wnd;

	//
	static std::array<IModule*, MT_MAX> modules;

};

