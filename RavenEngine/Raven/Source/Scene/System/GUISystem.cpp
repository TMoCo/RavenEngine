#include "GUISystem.h"
#include "Scene/Scene.h"
#include "Scene/Component/GUIComponent.h"

namespace Raven 
{
	GUISystem::GUISystem()
	{

	}
	GUISystem::~GUISystem()
	{

	}

	void GUISystem::OnInit()
	{

	}

	void GUISystem::OnUpdate(float dt, Scene* scene)
	{
		// Get a reference to the scene registry
		auto& registry = scene->GetRegistry();

		// Look for all GUIMenu component entities
		auto mGuiView = registry.view<GUIMenu>();
		for (auto entity : mGuiView)
		{
			const auto mGUI = registry.try_get<GUIMenu>(entity);
			if (mGUI) mGUI->UpdateGUI();
		}

		// Look for all GUIInGame component entities
		auto igGuiView = registry.view<GUIInGame>();
		for (auto entity : igGuiView)
		{
			const auto mGUI = registry.try_get<GUIInGame>(entity);
			if (mGUI) mGUI->UpdateGUI();
		}
	}

	void GUISystem::OnImGui()
	{

	}
}