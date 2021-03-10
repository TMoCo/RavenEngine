//

#include "Engine.h"
#include "IModule.h"
#include "Render/RenderModule.h"


#include "GL/glew.h"
#include "GLFW/glfw3.h"


#include <memory>






Engine::Engine()
{

}



Engine::~Engine()
{

}


Engine& Engine::Get()
{
	static std::unique_ptr<Engine> instance(CreateEngine());
	return *instance;
}


void Engine::Initialize()
{
    glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DOUBLEBUFFER, 8);
  glfwWindowHint(GLFW_SAMPLES, 0);

  glfw_window = glfwCreateWindow(1280, 1024, "Title", nullptr, nullptr);

  glfwMakeContextCurrent(glfw_window);


  // Module
  LoadModules();

}



int Engine::Run()
{
  static double enginetime = 0;

  // Main Loop...
  while (!glfwWindowShouldClose(glfw_window))
  {
    glfwPollEvents();

    //
    double dt = glfwGetTime() - enginetime;
    enginetime = glfwGetTime();

  

    // Draw..
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(glfw_window);
  }



  // Clean Up..
  DestoryModules();

  return 0;
}




void Engine::LoadModules()
{
  // Create...
  CreateModule<Raven::RenderModule>();


  // Initialize - Here order matter.
  InitializeModule<Raven::RenderModule>();
}



void Engine::DestoryModules()
{
  // Destroy - Here order matter.
  DestroyModule<Raven::RenderModule>();
}
