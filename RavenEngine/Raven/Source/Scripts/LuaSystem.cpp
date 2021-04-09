//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "LuaSystem.h"
#include "Scene/Scene.h"
#include "LuaComponent.h"
#include "Engine.h"

namespace Raven 
{
	void LuaSystem::OnInit()
	{
		
	}

	void LuaSystem::OnUpdate(float dt, Scene* scene)
	{
		if (Engine::Get().GetEditorState() == EditorState::Play) 
		{
			auto view = scene->GetRegistry().view<LuaComponent>();
			for (auto v : view)
			{
				auto& lua = scene->GetRegistry().get<LuaComponent>(v);
				lua.OnUpdate(dt);
			}
		}
	
	}

	void LuaSystem::OnImGui()
	{

	}
};