//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <imgui.h>
#include <memory>
#include "EditorWindow.h"
#include <entt/entt.hpp>
namespace Raven 
{
	class HierarchyWindow : public EditorWindow
	{
	public:
		HierarchyWindow();
		virtual void OnImGui()  override;
	private:
		void DrawName();
		void DrawFilter();
		void DragEntity();
		void DrawNode(const entt::entity & node, entt::registry& registry);
		ImGuiTextFilter hierarchyFilter;;
	};
};