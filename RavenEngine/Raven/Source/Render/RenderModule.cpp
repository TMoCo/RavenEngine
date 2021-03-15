#include "RenderModule.h"


#include "Engine.h"
#include "Window/Window.h"
#include "RenderDebug.h"
#include "OpenGL/GLContext.h"

#include "RenderObjects/RenderScene.h"
#include "Render/RenderObjects/RenderTerrain.h"
#include "Render/RenderResource/RenderRscTerrain.h"
#include "Render/RenderResource/RenderRscMaterial.h"
#include "Render/RenderResource/RenderRscShader.h"
#include "Render/RenderObjects/RenderTerrain.h"


#include "OpenGL/GLShader.h"
#include "OpenGL/GLFrameBuffer.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLRenderBuffer.h"
#include "OpenGL/GLTexture.h"


#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtx/transform.hpp"


#include <chrono>



using namespace Raven;






// ~TESTING-------------------------------------------------------
GLFrameBuffer* fbo = nullptr;
GLRenderBuffer* colorTarget = nullptr;
GLRenderBuffer* depthTarget = nullptr;

RenderRscTerrain* terrainRsc = nullptr;
RenderTerrain* terrain = nullptr;
RenderRscShader* terrainShader = nullptr;
RenderRscMaterial* terrainMaterail = nullptr;

static float camRot = 0.0f;

// ~TESTING-------------------------------------------------------






RenderModule::RenderModule()
	: isRendering(false)
{
	rdebug.reset(new RenderDebug());
}


RenderModule::~RenderModule()
{

}



void RenderModule::Initialize()
{
	// Load OpenGL Extensions.
	auto result = glewInit();
	RAVEN_ASSERT(result == 0, "Render - Faield to initialize glew.");

	// OpenGL Context...
	context = std::make_unique<GLContext>();
	context->SetGLFWContext( Engine::GetModule<Window>()->GetNativeWindow() );

	// We are only using one context, so make it current.
	context->MakeCurrent(); 

	// Setup Render Debug.
	rdebug->Setup();


	// ~TESTING-------------------------------------------------------
	colorTarget = GLRenderBuffer::Create(EGLFormat::RGBA, 820, 820);
	depthTarget = GLRenderBuffer::Create(EGLFormat::Depth32, 820, 820);

	fbo = GLFrameBuffer::Create();
	fbo->Attach(EGLAttachment::Color0, colorTarget);
	fbo->Attach(EGLAttachment::Depth, depthTarget);
	fbo->Update();


	unsigned char* heigtData = new unsigned char[1024 * 1024];

	for (int x = 0; x < 1024; ++x)
	{
		for (int y = 0; y < 1024; ++y)
		{
			float fx = x / (float)(1024 - 1);
			float fy = y / (float)(1024 - 1);

			float sx = cos(fx * 20.f) * 0.5f + 0.5f;
			float sy = sin(fy * 20.f) * 0.5f + 0.5f;
			float height = sx + sy;
			height = height * 0.5f;

			heigtData[x + y * 1024] = height * 255;
		}
	}


	glm::vec2 terrainScale = glm::vec2(1000.0f);

	terrainRsc = new RenderRscTerrain();
	terrainRsc->LoadHeightMap(1024, 1024, heigtData);
	terrainRsc->GenerateTerrain(100, terrainScale);
	delete heigtData;


	terrainShader = new RenderRscShader();
	terrainShader->Load(ERenderShaderType::Terrain, "TerrainShader");

	terrainMaterail = new RenderRscMaterial(terrainShader);

	terrain = new RenderTerrain();
	terrain->SetHeight(200.0f);
	terrain->SetTerrainRsc(terrainRsc);
	terrain->SetWorldMatrix( glm::translate(glm::vec3(terrainScale.x, 0.0f, terrainScale.y) * -0.5f) );
	terrain->SetNormalMatrix(glm::mat4(1.0f));
	terrain->SetMaterial(terrainMaterail);

	// ~TESTING-------------------------------------------------------

}


void RenderModule::Destroy()
{
	//
	rdebug->Destroy();


	delete terrain;
	delete terrainRsc;
	delete terrainShader;
	delete terrainMaterail;
	delete fbo;
	delete colorTarget;
	delete depthTarget;
}


void RenderModule::Update(float dt)
{
	// ~TESTING-------------------------------------------------------
	// Debug Drawing...
	static float debugtime = 0.0f;
	debugtime += dt;
	camRot += dt;


	if (debugtime > 0.1f)
	{
		static glm::vec3 pos;
		pos.x = (rand() / (float)RAND_MAX);
		pos.y = (rand() / (float)RAND_MAX);
		pos.z = (rand() / (float)RAND_MAX);

		pos = pos * 2.0f - 1.0f;
		pos = pos * 2.5f;

		// Random Yellow Boxes
		GetDebug()->DrawBox(glm::vec3(pos), glm::vec3(rand() / RAND_MAX + 0.5f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 1.0f);

		debugtime = 0.0f;
	}

	// Green Box
	GetDebug()->DrawBox(glm::vec3(0.0f), glm::vec3(5.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.0f);

	// ~TESTING-------------------------------------------------------

	rdebug->Update(dt);

}


void RenderModule::BeginRender()
{
	RAVEN_ASSERT(!isRendering, "Render - Trying to begin render before EndRender().");
	isRendering = true;

	// ~TESTING-------------------------------------------------------


	glm::mat4 view = glm::lookAt(glm::vec3(cos(camRot), 0.7f, sin(camRot)) * abs(sin(camRot * 0.3f)) * 1000.0f + 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.01f, 10000.0f);




	// ~TESTING-------------------------------------------------------


	// 
	rscene = std::make_shared<RenderScene>();
	rscene->SetView(view);
	rscene->SetProjection(proj);
	rscene->Build(terrain);
	rscene->AddDebugPrimitives( rdebug->GetRenderPrimitive() );
}


void RenderModule::Render()
{
	RAVEN_ASSERT(isRendering, "Render - Trying to render before BeginRender().");

	fbo->Bind(EGLFrameBuffer::Framebuffer);
	glViewport(0 ,0, 820, 820);
	glScissor(0, 0, 820, 820);
	glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	rscene->Draw(ERSceneBatch::Opaque);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	rscene->Draw(ERSceneBatch::Debug);

	fbo->Blit(
		nullptr,													// Default Framebuffer.
		EGLBufferMask::Color,							// Color Mask Bit. 
		EGLFilter::Nearest,								// Filter.
		FBBlitViewport(0, 0, 820, 820),		// Src Viewport.
		FBBlitViewport(0, 0, 820, 820)		// Dst Viewport.
	);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void RenderModule::EndRender()
{
	isRendering = false;

	// Cleanup Render Data...
	rscene->Clear();

}


RenderSurface RenderModule::GetRequiredRenderSurface()
{
	return GLContext::GetSurface();
}
