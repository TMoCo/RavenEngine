//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "ImportWindow.h"

#include "Engine.h"

#include "ResourceManager/FileSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Importers/FBXImporter.h"

#include "Animation/Skeleton.h"

#include "Logger/Console.h"

#include <imgui.h>
#include <ImGuiFD/ImGuiFileDialog.h>

namespace Raven
{
	ImportWindow::ImportWindow() :
		resourceManager(Engine::Get().GetModule<ResourceManager>()), // pointer to resource manager
		input("/"),
		onlyAnimation(false),
		selected(false),
		dragInfo("Drag skeleton here")
	{
		title = "Import";
	} 

	void ImportWindow::OnImGui()
	{
		if (ImGui::Begin(title.c_str(), &active))
		{

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			ImGui::Columns(1);
			ImGui::PushItemWidth(-1.0f); // means fill available space for any non button widget
			float w = ImGui::GetContentRegionAvailWidth(); // for the button to stretch full 

			if (ImGui::InputText("Select file", input, IM_ARRAYSIZE(input)))
			{
				// Open a file browser dialogue once library is included
				filePath = StringUtils::GetCurrentWorkingDirectory().append(input);
			}

			if (ImGui::Button("Testing file dialog"))
			{
				//igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");
			}

			if (StringUtils::GetExtension(filePath) == "fbx")
			{
				ImGui::Columns(2);
				ImGui::TextUnformatted("Animation only");
				ImGui::NextColumn();
				// if an fbx, determine if we want just the animation (if so, specify a skeleton)
				ImGui::Checkbox("##Animation only", &onlyAnimation);
				ImGui::Columns(1);

				if (onlyAnimation)
				{
					//ImGui::Button(dragInfo.c_str(), ImVec2(w, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(255, 0, 0, 100));
					ImGui::BeginChild("Drag area", ImVec2(w, 40.0f));
					ImGui::TextUnformatted(dragInfo.c_str());
					ImGui::EndChild();
					if (ImGui::BeginDragDropTarget())
					{
						auto data = ImGui::AcceptDragDropPayload("AssetFile");
						if (data)
						{
							PRINT_FUNC();
							std::string path = (char*)data->Data;
							if (Engine::GetModule<ResourceManager>()->GetResourceType(path) == EResourceType::RT_Skeleton)
							{
								dragInfo = path;
							}
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopStyleColor();
				}
			}

			ImGui::Separator();
			if (ImGui::Button("Import", ImVec2(w, 0.0f)))
			{
				LOGW("Path: {0}", filePath.c_str());
				if (onlyAnimation && dragInfo != "Drag here")
				{
					// in case of animation only, change the import settings of the fbx importer
					resourceManager->GetImporter<FBXImporter>()->settings.skeleton = resourceManager->GetResource<Skeleton>(dragInfo);
					resourceManager->GetImporter<FBXImporter>()->settings.importAnimationOnly = true;
				}
				// Imports the resource
				resourceManager->Import(filePath);
				selected = false;
				dragInfo = "Drag here";
			}
			ImGui::PopStyleVar();
		}
		ImGui::End();
	}
};

