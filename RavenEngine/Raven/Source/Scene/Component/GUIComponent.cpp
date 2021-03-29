#include "GUIComponent.h"
#include "Window/Window.h"
#include "glm/glm.hpp"

namespace Raven
{
	GUIComponent::GUIComponent() 
	{
		regi = Engine::Get().GetModule<GUIModule>();

	}

	GUIMenu::GUIMenu() {
		/* The following simply defines the hard coded configuration for the main menu GUI
		 * This is only accepatble for the minimum solition and should be done by the game developer
		 * when they create the scene entity containing this component
		 * these config structs will be handles by the resource manager
		 */
		auto window = Engine::Get().GetModule<Window>();

		glm::vec2 wSize = window->GetWindowSize();

		titlePanelCFG.size = ImVec2(400, 70);
		titlePanelCFG.bg_col = ImVec4(1., 1., 1., 1.);
		titlePanelCFG.position = ImVec2((wSize.x - titlePanelCFG.size.x) / 2.f, 100);

		titleTextCFG.font = regi->getFont(1);
		titleTextCFG.color = ImVec4(0., 0., 0., 1.);
		titleTextCFG.text_align = ImVec2(.5, .5);

		startPanelCFG.size = ImVec2(300, 70);
		startPanelCFG.bg_col = ImVec4(0., 1., 1., 0.);
		startPanelCFG.position = ImVec2((wSize.x - startPanelCFG.size.x) / 2.f, 400);
		startPanelCFG.rounding = 0.;
		startPanelCFG.padding = ImVec2(0, 0);
		startPanelCFG.border_col = ImVec4(0, 0, 0, 0);

		startTextCFG.font = regi->getFont(1);
		startTextCFG.color = ImVec4(0., 0., 0., 1.);
		startTextCFG.text_align = ImVec2(.5, .5);

		startButtonCFG.size = ImVec2(300, 70);
		startButtonCFG.color = ImVec4(163. / 255., 250. / 255., 231. / 255., 255. / 255.);
		startButtonCFG.color_hover = ImVec4(91 / 255., 250. / 255., 231. / 255., 255. / 255.);
		startButtonCFG.color_active = ImVec4(74 / 255., 179 / 255., 255 / 255., 255. / 255.);

	}

	// This function will be responsible for defining the GUI
	void GUIMenu::UpdateGUI()
	{
		regi->BeginFrame();

		regi->BeginPanel(titlePanelCFG);
		regi->Text(titleTextCFG, "Raven Game");
		regi->EndPanel();

		regi->BeginPanel(startPanelCFG);
		if (regi->Button(startButtonCFG, startTextCFG, "Start Game")) {
			LOGI("The start button has been pressed!");
			// TODO: Add the scene switching code
			if (callback) 
			{
				callback();
			}
		}
		regi->EndPanel();

		regi->EndFrame();
	}

	void GUIMenu::SetButtonCallback(const std::function<void()>& callf)
	{
		callback = callf;
	}

	GUIInGame::GUIInGame()
	{
		auto window = Engine::Get().GetModule<Window>();
		glm::vec2 wSize = window->GetWindowSize();

		healthPanelCFG.size = ImVec2(200, 60);
		healthPanelCFG.position = ImVec2(0, (wSize.y - healthPanelCFG.size.y));
		healthPanelCFG.bg_col = ImVec4(0., 0., 0., 0.5);
		healthPanelCFG.rounding = 0.;
		healthPanelCFG.border_col = ImVec4(0, 0, 0, 0);

		healthTextCFG.font = regi->getFont(1);
		healthTextCFG.color = ImVec4(1., 1., 1., 1.);
		healthTextCFG.text_align = ImVec2(0, 0);

		samplePanelCFG = healthPanelCFG;
		samplePanelCFG.size = ImVec2(270, 65);
		samplePanelCFG.position = ImVec2((wSize.x - samplePanelCFG.size.x), (wSize.y - samplePanelCFG.size.y));

		sampleTextCFG = healthTextCFG;
	}

	void GUIInGame::UpdateGUI() 
	{
		regi->BeginFrame();

		regi->BeginPanel(healthPanelCFG);
		regi->Text(healthTextCFG, "100 HP");
		regi->EndPanel();

		regi->BeginPanel(samplePanelCFG);
		regi->Text(sampleTextCFG, "Sample Text");
		regi->EndPanel();


		regi->EndFrame();

	}

}