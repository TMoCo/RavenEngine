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

#include "Scene/Component/GUIComponent.h"

namespace Raven
{
	void RavenGame::Initialize()
	{
		Raven::Engine::Initialize();
		//init code here for game 

		auto scene = new Scene("Main Menu");
		GetModule<SceneManager>()->AddScene(scene);
		scene->OnInit();


		auto guiEnt = scene->CreateEntity("GUI_Entity");
		
		guiEnt.AddComponent<GUIMenu>();
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