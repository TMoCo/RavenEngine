#include <iostream>


#include "Engine.h"
#include "Render/RenderModule.h"





int main(int argc, char** argv) 
{
	Engine::Get().Initialize();


	// Example how to access modules from engine.
	Engine::GetModule<RenderModule>()->Example();


	return Engine::Get().Run();
}

