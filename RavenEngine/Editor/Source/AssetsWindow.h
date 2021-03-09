//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <imgui.h>
#include <memory>

#include "EditorWindow.h"

namespace Raven 
{
	class AssetsWindow : public EditorWindow
	{
	public:
		AssetsWindow();
		virtual void OnImGui() override;

		

	};
};