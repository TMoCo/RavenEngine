#pragma once
//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "IModule.h"
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}


#include <string>

namespace Raven 
{
	class LuaVirtualMachine final : public IModule
	{
	public:
		LuaVirtualMachine();
		void Initialize() override;
		void Destroy() override;
		inline auto GetState() { return L; }

		static auto GetModuleType() { return MT_LuaVirtualMachine; }

	private:
		void AddSystemPath(const std::string& path);
		void AddPath(const std::string& path);

		lua_State * L = nullptr;
	};
};