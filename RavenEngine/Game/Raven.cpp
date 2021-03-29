#include "Raven.h"
#include "EntryPoint.h"
#include "Engine.h"
#include "Logger/Console.h"
#include "GUI/GUIModule.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Entity/Entity.h"
#include "Scene/System/SystemManager.h"
#include "Scene/System/GUISystem.h"

#include "Scene/Component/GUIComponent.h"

namespace Raven
{
	void RavenGame::Initialize()
	{
		Raven::Engine::Initialize();
		//init code here for game 
		auto scene = new Scene("default");
		GetModule<SceneManager>()->AddScene(scene);
		GetModule<SceneManager>()->AddSceneFromFile("scenes/deer2.raven");
		auto deerScene = GetModule<SceneManager>()->GetSceneByName("deer2");

		//more safety way
		scene->SetInitCallback([](Scene * _scene) {
			auto guiEnt = _scene->CreateEntity("GUI_Entity");
			auto& menu = guiEnt.AddComponent<GUIMenu>();

			menu.SetButtonCallback([]() {
				GetModule<SceneManager>()->SwitchScene("deer2");
			});

		});

		deerScene->SetInitCallback([](Scene* _scene) {
			auto guiEnt = _scene->CreateEntity("GUI_Entity");
			auto& menu = guiEnt.AddComponent<GUIInGame>();
		});

	}

	void RavenGame::OnImGui() 
	{
		Engine::OnImGui();
	}

	void RavenGame::OnUpdate(float dt)
	{
		Engine::OnUpdate(dt);
	}

}

Raven::Engine* CreateEngine() 
{
	return new Raven::RavenGame();
}