//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "Animator.h"
#include "Utilities/StringUtils.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "ResourceManager/ResourceManager.h"

#include "Animation.h"
#include "AnimationController.h"

#include "ImGui/ImGuiHelpers.h"




namespace Raven
{
	void Animator::OnImGui()
	{
		if (ImGui::BeginDragDropTarget())
		{
			auto data = ImGui::AcceptDragDropPayload("AssetFile");
			if (data)
			{
				std::string file = (char*)data->Data;

				if (Engine::GetModule<ResourceManager>()->GetResourceType(file) == RT_AnimationController)
				{
					controller = Engine::GetModule<ResourceManager>()->GetResource<AnimationController>(file);
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGuiHelper::Property("Apply Root Motion", rootMotion);
	}


};