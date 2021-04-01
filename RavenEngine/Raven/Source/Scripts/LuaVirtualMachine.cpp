//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "LuaVirtualMachine.h"
#include "Logger/Console.h"
#include <LuaBridge/LuaBridge.h>
#include <functional>

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

		//export function
		luabridge::getGlobalNamespace(L)
			.addFunction("LOGE", std::function <void(const std::string&)>([](const std::string& str) {
				LOGE(str);
			}))
			.addFunction("LOGV", std::function <void(const std::string&)>([](const std::string& str) {
				LOGV(str);
				}))
			.addFunction("LOGI", std::function <void(const std::string&)>([](const std::string& str) {
					LOGI(str);
				}))
			.addFunction("LOGC", std::function <void(const std::string&)>([](const std::string& str) {
						LOGC(str);
			}));


				luaL_dofile(L, "scripts/Test.lua");//test
				luaL_dofile(L, "scripts/Test2.lua");//test
				{
					auto ref = luabridge::getGlobal(L, "Test2");
					auto type = ref.type();

					ref["OnTest"]();

				} 
				{
					//get lua table ( you can think it is a object )
					auto ref = luabridge::getGlobal(L, "Test");
					auto type = ref.type();
					ref["OnTest"]();
				}

	}

	void LuaVirtualMachine::Destroy()
	{
		lua_close(L);
	}

};
