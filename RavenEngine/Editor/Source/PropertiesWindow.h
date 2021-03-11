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
	
	class PropertiesWindow : public EditorWindow
	{
	public:
		PropertiesWindow();
		virtual void OnImGui()  override;
	};
};