//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "LogExport.h"
#include "Scripts/LuaVirtualMachine.h"
#include <LuaBridge/LuaBridge.h>
#include "Console.h"


namespace Raven
{
	namespace LogExport
	{
	
		void Export(lua_State* L)
		{
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

		}
	};
};
