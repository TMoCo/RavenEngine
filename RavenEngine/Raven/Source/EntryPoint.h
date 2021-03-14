#include <iostream>


#include "Engine.h"
#include "Render/RenderModule.h"
#include "Logger/Console.h"

extern Engine * CreateEngine();

int main(int argc, char** argv) 
{
	Raven::Console::Init();
	Engine::Get().Initialize();


	// Example how to access modules from engine.
	Engine::GetModule<RenderModule>()->Example();
	Engine::GetModule<RenderModule>()->Example();


	return Engine::Get().Run();
}

