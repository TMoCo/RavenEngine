#include "Raven.h"
#include "EntryPoint.h"
#include "Engine.h"
#include "Logger/Console.h"
#include "GUI/GUIModule.h"

#include "ResourceManager/ResourceManager.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Entity/Entity.h"
#include "Scene/System/SystemManager.h"
#include "Scene/System/GUISystem.h"

#include "Scene/Component/GUIComponent.h"
#include "ResourceManager/Resources/Texture2D.h"


namespace Raven
{
	void RavenGame::Initialize()
	{
		Raven::Engine::Initialize();

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