//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "AssetsWindow.h"

namespace Raven 
{

	AssetsWindow::AssetsWindow()
	{
		title = "Assets";
	}

	void AssetsWindow::OnImGui()
	{
		ImGui::Begin(title.c_str(), &active);

		ImGui::End();
	}

};

