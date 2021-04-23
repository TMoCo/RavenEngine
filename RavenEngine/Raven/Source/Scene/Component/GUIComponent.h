#pragma once
#include "GUI/GUIModule.h"
#include "GUI/GUILayout.h"
#include "Engine.h"
#include <functional>

namespace Raven
{
	class GUIComponent
	{
	public:
		GUIComponent();
		void setLayout(GUILayout* _layout);
		void UpdateGUI();

	protected:
		// Reference to the GUIModule
		GUIModule* regi;
		GUILayout* layout;
	};
};