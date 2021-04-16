//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <imgui.h>
#include <memory>
#include "EditorWindow.h"
#include <entt/entt.hpp>
#include "ofbx.h"

namespace Raven
{
	class FbxWindow : public EditorWindow
	{
	public:
		FbxWindow();
		void OpenFile(const std::string& name);
		void ShowObjectGUI(const ofbx::Object& scene);
		virtual void OnImGui()  override;

	private:
		ofbx::IScene* scene = nullptr;
		const ofbx::Object* currentSelect = nullptr;
	};
};