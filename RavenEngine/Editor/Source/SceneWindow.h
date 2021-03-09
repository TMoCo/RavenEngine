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
	class SceneWindow : public EditorWindow
	{
	public:
		SceneWindow();
		virtual void OnImGui()  ;
		virtual void Resize(uint32_t width, uint32_t height);
	private:
		uint32_t width;
		uint32_t height;
	};
};