//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "AnimationSystem.h"
#include "Animator.h"
#include "Animation.h"
#include "AnimationController.h"
#include "Engine.h"


#include "Scene/Component/SkinnedMeshComponent.h"


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

				if (!animator.controller)
					continue;

				Entity skinnedEnttity{ e, scene };
				SkinnedMeshComponent* skinnedComp = skinnedEnttity.TryGetComponent<SkinnedMeshComponent>();

				if (skinnedComp)
				{
					animator.controller->OnUpdate(dt, skinnedComp);
				}
			}
		}
	}

	void AnimationSystem::OnImGui()
	{
	}

};