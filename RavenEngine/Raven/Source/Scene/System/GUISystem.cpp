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
		auto guiView = registry.view<GUIComponent>();
		for (auto entity : guiView)
		{
			const auto mGUI = registry.try_get<GUIComponent>(entity);
			if (mGUI) mGUI->UpdateGUI();
		}
	}

	void GUISystem::OnImGui()
	{

	}
}