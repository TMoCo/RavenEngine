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

				Entity skinnedEnttity{ e, scene };
				SkinnedMeshComponent* skinnedComp = skinnedEnttity.TryGetComponent<SkinnedMeshComponent>();

				if (animator.isSimpleAnimator)
				{
					if (animator.anim)
					{
						if (!animator.anim->IsStarted())
						{
							animator.anim->Play(0, skinnedComp->GetSkeleton());

							if (animator.offset > 0.000001f)
								animator.anim->OnUpdate(animator.offset);
						}
						else
						{
							animator.anim->OnUpdate(dt);
						}
					}
				}
				else
				{
					if (animator.controller && skinnedComp)
					{
						animator.controller->OnUpdate(dt, skinnedComp);
					}
				}
				
			}
		}
	}

	void AnimationSystem::OnImGui()
	{
	}

};