//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "LuaVirtualMachine.h"
#include "Logger/Console.h"
#include <LuaBridge/LuaBridge.h>
#include <functional>
#include "LuaComponent.h"
#include "Devices/InputExport.h"
#include "MathExport.h"
#include "Devices/Input.h"
#include "Logger/LogExport.h"
#include "ComponentExport.h"

namespace Raven 
{

	LuaVirtualMachine::LuaVirtualMachine()
	{

	}

	void LuaVirtualMachine::Initialize()
	{
		PRINT_FUNC();
		L = luaL_newstate();
		luaL_openlibs(L);//load all default lua functions

		InputExport::Export(L);
		LogExport::Export(L);
		MathExport::Export(L);
		ComponentExport::Export(L);
	}

	void LuaVirtualMachine::Destroy()
	{
		lua_close(L);
	}

};
