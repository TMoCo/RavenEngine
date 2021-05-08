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
		std::string animctrlAsset = controllerInstance ? controllerInstance->GetParentController()->GetResourcePath() : "";
		ImGuiHelper::Property("Animation Controller", animctrlAsset);

		if (ImGui::BeginDragDropTarget())
		{
			auto data = ImGui::AcceptDragDropPayload("AssetFile");
			if (data)
			{
				std::string file = (char*)data->Data;

				if (Engine::GetModule<ResourceManager>()->GetResourceType(file) == RT_AnimationController)
				{
					auto controller = Engine::GetModule<ResourceManager>()->GetResource<AnimationController>(file);
					controllerInstance = Ptr<AnimationControllerInstance>(new AnimationControllerInstance(controller));
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGuiHelper::Property("Apply Root Motion", rootMotion);
	}


};