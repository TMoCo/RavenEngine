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
public:
  /** */
	Engine();

public:
	/** */
	~Engine();

	/** */
	static Engine& Get();

	/** */
	virtual void Initialize();

	/** */
	int32_t Run();

	/** */
	template<class ModuleType>
	inline ModuleType* GetModule()
	{
		return static_cast<ModuleType*>( modules[ ModuleType::GetType() ] );
	}

private:
	/** */
	void LoadModules();




private:
	GLFWwindow* wnd;

	//
	std::array<IModule*, MT_MAX> modules;

};

Engine* CreateEngine();

