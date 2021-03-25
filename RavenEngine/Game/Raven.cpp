#include "Raven.h"
#include "EntryPoint.h"
#include "Engine.h"
#include "Logger/Console.h"
#include "GUI/GUIModule.h"

void RavenGame::Initialize()
{
	Raven::Engine::Initialize();
	//init code here for game 
}

void RavenGame::OnImGui() {
	Raven::Engine::OnImGui();
}

Raven::Engine* CreateEngine() {
	return new RavenGame();
}
