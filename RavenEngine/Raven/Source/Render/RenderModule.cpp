#include "RenderModule.h"


#include "Engine.h"
#include "OpenGL/GLContext.h"
#include "OpenGL/GLShader.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"




using namespace Raven;






RenderModule::RenderModule()
	: isRendering(false)
{

}


RenderModule::~RenderModule()
{

}



void RenderModule::Initialize()
{
	glewInit();


	context.reset( new GLContext() );


	GLShader* tmp = GLShader::Create("Test_Shader");
	tmp->SetSourceFile(EGLShaderStage::Vertex, "C:/Temp/Vert.glsl");
	tmp->SetSourceFile(EGLShaderStage::Fragment, "C:/Temp/Frag.glsl");
	tmp->AddExSource("T0", EGLShaderStage::Vertex, "void FuntionTest()\n{\n\tvec4 x(1.0);\n}\n");
	tmp->AddPreprocessor("#define VERT_FLAG 0");
	tmp->Build();

}


void RenderModule::Destroy()
{

}


void RenderModule::BeginRender()
{
	isRendering = true;

}


void RenderModule::Render()
{

}


void RenderModule::EndRender()
{
	isRendering = false;

}
