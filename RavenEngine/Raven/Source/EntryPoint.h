#include <iostream>


#include "Engine.h"
#include "Render/RenderModule.h"
#include "Logger/Console.h"

extern Raven::Engine * CreateEngine();

int main(int argc, char** argv) 
{
	Raven::Console::Init();
	Raven::Engine::Get().Initialize();


	// Example how to access modules from engine.
	Raven::Engine::GetModule<RenderModule>()->Example();


	return Raven::Engine::Get().Run();
}

