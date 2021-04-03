//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <imgui.h>
#include <memory>

#include "EditorWindow.h"
#include <ImGuiEnttEditor.hpp>

namespace Raven 
{
	
	class PropertiesWindow : public EditorWindow
	{
	public:
		PropertiesWindow();
		virtual void OnImGui() override;
		virtual void OnSceneCreated(Scene* scene) override;
	private:
		bool init = false;
		//MM::ImGuiEntityEditor<entt::entity> enttEditor;
		MM::EntityEditor<entt::entity> enttEditor;
	};
};