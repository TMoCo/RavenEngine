#include <iostream>


#include "Engine.h"
#include "Render/RenderModule.h"



int main(int argc, char** argv) 
{
	Engine::Get().Initialize();

	Engine::GetModule<RenderModule>()->Destroy();

	return Engine::Get().Run();
}