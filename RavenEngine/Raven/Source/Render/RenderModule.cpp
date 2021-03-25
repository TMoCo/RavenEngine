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


#include "Scene/Scene.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"
#include "Scene/SceneGraph.h"
#include <entt/entt.hpp>


#include <chrono>



using namespace Raven;






// ~TESTING-------------------------------------------------------
GLFrameBuffer* fbo = nullptr;
GLTexture* colorTarget = nullptr;
GLRenderBuffer* depthTarget = nullptr;

RenderRscTerrain* terrainRsc = nullptr;
RenderTerrain* terrain = nullptr;
RenderRscShader* terrainShader = nullptr;
RenderRscMaterial* terrainMaterail = nullptr;

static float camRot = 0.0f;
static bool blitToWindow = false;
static glm::ivec2 targetSize = glm::ivec2(50, 50);


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
	colorTarget = GLTexture::Create(EGLTexture::Texture2D, EGLFormat::RGBA);
	colorTarget->Bind();
	colorTarget->SetFilter(EGLFilter::Nearest);
	colorTarget->SetFilter(EGLFilter::Nearest);
	colorTarget->UpdateTexData(0, 820, 820, nullptr);
	colorTarget->UpdateTexParams();
	colorTarget->Unbind();

	depthTarget = GLRenderBuffer::Create(EGLFormat::Depth32, 820, 820);

	fbo = GLFrameBuffer::Create();
	fbo->Attach(EGLAttachment::Color0, 0, colorTarget);
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
	camRot += dt * 0.5f;

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
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.0f);

	// ~TESTING-------------------------------------------------------

	rscene = std::make_shared<RenderScene>();
	rdebug->Update(dt);

}


void RenderModule::BeginRender(Scene* scene, bool blit, const glm::ivec2& extent)
{
	RAVEN_ASSERT(!isRendering, "Render - Trying to begin render before EndRender().");
	isRendering = true;
	blitToWindow = blit;


	// ~TESTING-------------------------------------------------------

	if (blitToWindow)
	{
		if (targetSize.x != extent.x || targetSize.y != extent.y)
		{
			// Invalid Size?
			if (extent.x < 10 || extent.y < 10)
				targetSize = glm::max(extent, glm::ivec2(10, 10));
			else
				targetSize = extent;


			// Resize Target...
			colorTarget->Bind();
			colorTarget->UpdateTexData(0, targetSize.x, targetSize.y, nullptr);
			colorTarget->Unbind();

			depthTarget->Bind();
			depthTarget->UpdateStorage(EGLFormat::Depth32, targetSize.x, targetSize.y);
			depthTarget->Unbind();
		}
	}

	glm::mat4 view = glm::lookAt(glm::vec3(cos(camRot), 0.7f, sin(camRot)) * abs(sin(camRot * 0.03f)) * 1000.0f + 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)targetSize.x / (float)targetSize.y, 0.01f, 10000.0f);
	// ~TESTING-------------------------------------------------------


	// ~TESTING SCENE DRAWING------------------------------------------
	auto& registry = scene->GetRegistry();
	auto entityView = registry.view<Transform>(entt::exclude<Hierarchy>);

	for (auto entity : entityView)
	{
		auto& transform = entityView.get<Transform>(entity);

		GetDebug()->DrawBox(transform.GetWorldPosition(), glm::vec3(1.0f),
			glm::vec4(1.0f));
	}

	// ~TESTING SCENE DRAWING------------------------------------------


	// 
	rscene->SetView(view);
	rscene->SetProjection(proj);
	rscene->Build(terrain, scene);
	rscene->AddDebugPrimitives( rdebug->GetRenderPrimitive() );
}


void RenderModule::Render()
{
	RAVEN_ASSERT(isRendering, "Render - Trying to render before BeginRender().");

	fbo->Bind(EGLFrameBuffer::Framebuffer);
	glViewport(0 ,0, targetSize.x, targetSize.y);
	glScissor(0, 0, targetSize.x, targetSize.y);
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


	if (blitToWindow)
	{
		fbo->Blit(
			nullptr,													// Default Framebuffer.
			EGLBufferMask::Color,							// Color Mask Bit. 
			EGLFilter::Nearest,								// Filter.
			FBBlitViewport(0, 0, targetSize.x, targetSize.y),		// Src Viewport.
			FBBlitViewport(0, 0, targetSize.x, targetSize.y)		// Dst Viewport.
		);
	}


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


GLTexture* RenderModule::GetSceneRT()
{
	return colorTarget;
}
