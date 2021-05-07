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

		char input[StringUtils::MAX_PATH_LENGTH];
		
		bool onlyAnimation;
		bool selected;

		std::string dragInfo;

		// file info
		std::string fileName;
		std::string filePath;
	};
};