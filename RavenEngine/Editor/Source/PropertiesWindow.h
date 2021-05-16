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
	
	class AnimationController;
	class PropertiesWindow : public EditorWindow
	{
	public:
		PropertiesWindow();
		virtual void OnImGui() override;
		virtual void OnSceneCreated(Scene* scene) override;
		inline void SetController(Ptr<AnimationController> con) { controller = con; }

		static std::string ImGuiDragDropAssetsTarget();

		//
		void OnImGUIGlobalSettings();

	private:
		bool init = false;
		MM::EntityEditor<entt::entity> enttEditor;
		WeakPtr<AnimationController> controller;
	};
};