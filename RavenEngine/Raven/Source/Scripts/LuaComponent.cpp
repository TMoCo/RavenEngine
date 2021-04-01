//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "LuaComponent.h"
#include "LuaVirtualMachine.h"
#include "Engine.h"


namespace Raven 
{
	LuaComponent::LuaComponent(const std::string& file, Scene* scene)
	{

	}

	LuaComponent::LuaComponent()
	{

	}

	LuaComponent::~LuaComponent()
	{

	}

	void LuaComponent::OnInit()
	{

	}

	void LuaComponent::OnUpdate(float dt)
	{

	}

	void LuaComponent::Reload()
	{

	}

	void LuaComponent::Load(const std::string& fileName)
	{
		file = fileName;
	}

	void LuaComponent::LoadScript()
	{
		luaL_dofile(Engine::Get().GetModule<LuaVirtualMachine>()->GetState(), file.c_str());
	}

	void LuaComponent::Init()
	{
		LoadScript();
	}

};