//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "AnimationSystem.h"
#include "Animator.h"
#include "Animation.h"
#include "AnimationController.h"
#include "Engine.h"
namespace Raven
{

	AnimationSystem::AnimationSystem()
	{
	}

	void AnimationSystem::OnInit()
	{
	}

	void AnimationSystem::OnUpdate(float dt, Scene* scene)
	{
		if (Engine::Get().GetEditorState() == EditorState::Play) 
		{
			auto animators = scene->GetRegistry().view<Animator>();

			for (auto e : animators)
			{
				auto& animator = scene->GetRegistry().get<Animator>(e);
				if (animator.controller != nullptr)
				{
					animator.controller->OnUpdate(dt, scene, e);
				}
			}
		}
	}

	void AnimationSystem::OnImGui()
	{
	}

};