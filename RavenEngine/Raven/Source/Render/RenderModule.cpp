#include "RenderModule.h"


#include "Engine.h"
#include "Window/Window.h"
#include "RenderDebug.h"
#include "OpenGL/GLContext.h"
#include "RenderTarget.h"

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


#include "Core/Camera.h"

namespace Raven {






// ~TESTING-------------------------------------------------------

static float camRot = 0.0f;

// ~TESTING-------------------------------------------------------






RenderModule::RenderModule()
	: isRendering(false)
	, isRTToWindow(true)
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


	// Main Scene Render Target.
	rtScene = std::make_shared<RenderTarget>(EGLTexture::Texture2D, EGLFormat::RGBA);
	rtScene->Resize(glm::ivec2(1920, 1080));
	rtScene->SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f));
	rtScene->SetSampleProeprties(EGLFilter::Nearest, EGLWrap::ClampToEdge);
	rtScene->Update();

	// Render Scene.
	rscene = std::make_shared<RenderScene>();
	rscene->Setup();
}


void RenderModule::Destroy()
{
	//
	rdebug->Destroy();
	rtScene.reset();

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

	rdebug->Update(dt);

}


void RenderModule::BeginRender(Scene* scene, const glm::ivec2& extent)
{
	RAVEN_ASSERT(!isRendering, "Render - Trying to begin render before EndRender().");
	isRendering = true;

	// Should we resize with window?
	if (isRTToWindow)
	{
		// Resize our render target, it is fine to call it every frame as it only resize the actuall image when it get larger
		rtScene->Resize(extent);
	}

	// Update Render Target if dirty.
	if (rtScene->IsDirty())
		rtScene->Update();


	// ~TESTING-------------------------------------------------------
	//glm::mat4 view = glm::mat4(1.f);
	glm::mat4 view = glm::lookAt(glm::vec3(cos(camRot), 0.7f, sin(camRot)) * 1000.0f, 
		glm::vec3(500.0f, 0.0f, 500.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), rtScene->GetAspectRatio(), 0.01f, 10000.0f);
	
	rscene->SetView(view);
	rscene->SetProjection(proj);

	// ~TESTING-------------------------------------------------------



	// Build Render Data form the scene...
	rscene->AddDebugPrimitives( rdebug->GetRenderPrimitive() );
	rscene->Build(scene);
}


void RenderModule::Render()
{
	RAVEN_ASSERT(isRendering, "Render - Trying to render before BeginRender().");

	// Global OpenGL States...
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Bind FBO For drawing...
	rtScene->GetFBO()->Bind(EGLFrameBuffer::Framebuffer);
	
	// Viewport.
	glViewport(0 ,0, rtScene->GetSize().x, rtScene->GetSize().y);
	glScissor(0, 0, rtScene->GetSize().x, rtScene->GetSize().y);

	// Clear...
	glm::vec4 clearColor = rtScene->GetClearColor();
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	rscene->Draw(ERSceneBatch::Opaque);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	rscene->Draw(ERSceneBatch::Debug);


	if (isRTToWindow)
	{
		rtScene->GetFBO()->Blit(
			nullptr,													// Default Framebuffer.
			EGLBufferMask::Color,							// Color Mask Bit. 
			EGLFilter::Nearest,								// Filter.
			FBBlitViewport(0, 0, rtScene->GetSize().x, rtScene->GetSize().y),		// Src Viewport.
			FBBlitViewport(0, 0, rtScene->GetSize().x, rtScene->GetSize().y)		// Dst Viewport.
		);
	}

	rtScene->GetFBO()->Unbind(EGLFrameBuffer::Framebuffer);

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





} // End of namespace Raven.

