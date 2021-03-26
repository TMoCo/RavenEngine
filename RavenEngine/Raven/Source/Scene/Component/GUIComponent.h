#pragma once
#include "GUI/GUIModule.h"
#include "Engine.h"

namespace Raven
{
	class GUIComponent
	{
	public:
		GUIComponent();
		virtual void UpdateGUI() = 0;

	protected:
		// Reference to the GUIModule
		GUIModule* regi;


	};

	class GUIMenu : GUIComponent {
	public:
		GUIMenu();
		void UpdateGUI() override;

	private:
		// Hard coded configurations for the main menu GUI
		GUIModule::GUIPanelConfig titlePanelCFG;
		GUIModule::GUITextConfig titleTextCFG;
		GUIModule::GUIPanelConfig startPanelCFG;
		GUIModule::GUITextConfig startTextCFG;
		GUIModule::GUIButtonConfig startButtonCFG;
	};
};