#include <iostream>


#include "Engine.h"
#include "Render/RenderModule.h"
#include "Logger/Console.h"

extern Engine * CreateEngine();

int main(int argc, char** argv) 
{
	Raven::Console::Init();
	Engine::Get().Initialize();

	//Engine::GetModule<RenderModule>()->Destroy();

	return Engine::Get().Run();
}