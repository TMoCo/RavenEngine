#include "Raven.h"
#include "EntryPoint.h"
#include "Engine.h"
#include "Logger/Console.h"
#include "GUI/GUIModule.h"

void RavenGame::Initialize()
{
	Raven::Engine::Initialize();
	//init code here for game 
	/*LOGE("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGC("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGW("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGI("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGV("this is a message,{0},{1},{2}", 1, 2, 3);*/
}

void RavenGame::OnImGui() {
	Raven::Engine::OnImGui();
}

Raven::Engine* CreateEngine() {
	return new RavenGame();
}
