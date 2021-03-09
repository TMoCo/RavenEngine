
#pragma once
#include "Engine.h"
#include "EditorWindow.h"

namespace Raven 
{

	class Editor : public Engine
	{
	public:
		void Initialize() override;
		void OnImGui() override;
	private:
		void DrawMenu();
		void BeginDockSpace();
		void EndDockSpace();
		std::vector<std::unique_ptr<EditorWindow>> editorWindows;
	};




};
