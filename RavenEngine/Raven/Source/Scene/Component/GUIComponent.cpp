#include "GUIComponent.h"
#include "Window/Window.h"
#include "glm/glm.hpp"

namespace Raven
{
	GUIComponent::GUIComponent() 
	{
		regi = Engine::Get().GetModule<GUIModule>();
	}

	void GUIComponent::UpdateGUI()
	{
		regi->BeginFrame();
		layout->drawLayout();
		regi->EndFrame();
	}

	void GUIComponent::setLayout(GUILayout* _layout)
	{
		layout = _layout;
	}

}