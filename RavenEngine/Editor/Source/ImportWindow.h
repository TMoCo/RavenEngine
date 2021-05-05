//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma  once

#include "EditorWindow.h"

#include "ResourceManager/ResourceManager.h"

#include "Utilities/StringUtils.h"

#include <string>
#include <memory>

namespace Raven
{
	class ImportWindow : public EditorWindow
	{
		friend ResourceManager; // a friend of the resource manager class
	public:
		ImportWindow();
		virtual void OnImGui()  override;

	private:
		Ptr<ResourceManager> resourceManager;

		// this library keeps an instance of the file browser dialogue, sort of goes against immediate mode gui but whatever
		//ImGui::FileBrowser fileDialog;

		// vector of supported extensions
		//std::vector<std::string> extensions;

		char input[StringUtils::MAX_PATH_LENGTH];
		
		bool onlyAnimation;
		bool selected;

		std::string dragInfo;

		// file info
		std::string fileName;
		std::string filePath;
	};
};