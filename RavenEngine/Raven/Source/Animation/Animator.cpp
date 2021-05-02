//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "Animator.h"
#include "ImGui/ImGuiHelpers.h"
#include "Utilities/StringUtils.h"
#include "ResourceManager/FbxLoader_deprecated.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/Component/Model_deprecated.h"
#include "Animation.h"
#include "AnimationController.h"

namespace Raven
{
	void Animator::OnImGui()
	{
		ImGuiHelper::Property("Controller", animFile, true);

		if (ImGui::BeginDragDropTarget())
		{
			auto data = ImGui::AcceptDragDropPayload("AssetFile");
			if (data)
			{
				std::string file = (char*)data->Data;
				PRINT_FUNC();
				LOGV("Receive file from assets window : {0}", file);
				if (StringUtils::GetExtension(file) == "controller")
				{
					animFile = file;
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGuiHelper::Property("Apply Root Motion", rootMotion);
	}

	void Animator::CreateController()
	{
		controller = std::make_shared<AnimationController>(animFile);
	}

};