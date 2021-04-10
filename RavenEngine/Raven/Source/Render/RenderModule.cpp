#include "RenderModule.h"


#include "Engine.h"
#include "Window/Window.h"

#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/MaterialShader.h"
#include "ResourceManager/Resources/Material.h"
#include "ResourceManager/Resources/Texture2D.h"


#include "RenderDebug.h"
#include "RenderTarget.h"
#include "RenderPipeline.h"
#include "Render/RenderResource/Shader/RenderRscShader.h"
#include "Render/RenderResource/Shader/UniformBuffer.h"
#include "RenderObjects/RenderScene.h"
#include "RenderObjects/RenderPass.h"
#include "RenderObjects/RenderScreen.h"


#include "OpenGL/GLContext.h"
#include "OpenGL/GLShader.h"
#include "OpenGL/GLFrameBuffer.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLRenderBuffer.h"
#include "OpenGL/GLTexture.h"



#include "Scene/Scene.h"
#include "Scene/SceneGraph.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"



#include "entt/entt.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"









namespace Raven {








RenderModule::RenderModule()
	: isRendering(false)
	, isRTToWindow(true)
{
	rdebug.reset(new RenderDebug());
	pipeline.reset(new RenderPipeline());
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

	// Initialize the pipeline.
	pipeline->Initialize();


	// ...
	CreateDefaultMaterials();


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
	// Destroy Debug Render.
	rdebug->Destroy();
	
	// Destroy Main Render Target.
	rtScene.reset();

	// Destroy pipeline
	pipeline->Destroy();
}


void RenderModule::Update(float dt)
{
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
	float near = 0.01f, far = 10000.0f;
	float camRot = Engine::Get().GetEngineTime();
	camRot *= 0.1f;

#if 1
	glm::mat4 view = glm::lookAt(glm::vec3(cos(camRot), 0.7f, sin(camRot)) * 1000.0f, 
		glm::vec3(500.0f, 0.0f, 500.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), rtScene->GetAspectRatio(), near, far);
	
#else
	glm::mat4 view = glm::lookAt(glm::vec3(cos(camRot), 0.7f, sin(camRot)) * 10.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), rtScene->GetAspectRatio(), near, far);
#endif

	rscene->SetView(view);
	rscene->SetProjection(proj, near, far);

	// ~TESTING-------------------------------------------------------


	// Build Render Data form the scene...
	rscene->Build(scene);
	rscene->AddDebugPrimitives( rdebug->GetRenderPrimitive() );
}


void RenderModule::Render()
{
	RAVEN_ASSERT(isRendering, "Render - Trying to render before BeginRender().");

	// Engine Time.
	float time = Engine::Get().GetEngineTime();

	// Begin The Pipeline for rendering a scene.
	pipeline->Begin(rtScene.get(), rscene.get(), time);

	// Rendering...
	pipeline->Render();

	// End The Pipeline Render.
	pipeline->End();


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

	// Unbind FBO...
	rtScene->GetFBO()->Unbind(EGLFrameBuffer::Framebuffer);

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


void RenderModule::CreateDefaultMaterials()
{
	// Checker Texture...
	Engine::GetModule<ResourceManager>()->LoadResource<Texture2D>("assets/textures/T_Checker.png");
	Ptr<Texture2D> checkerTexture = Engine::GetModule<ResourceManager>()->GetResource<Texture2D>("assets/textures/T_Checker.png");
	checkerTexture->renderRscTexture = new RenderRscTexture();
	checkerTexture->LoadOnGpu();

	// TODO: Better texture system.
	checkerTexture->renderRscTexture->GetTexture()->Bind();
	checkerTexture->renderRscTexture->GetTexture()->SetWrap(EGLWrap::Repeat);
	checkerTexture->renderRscTexture->GetTexture()->GenerateMipmaps();
	checkerTexture->renderRscTexture->GetTexture()->UpdateTexParams();
	checkerTexture->renderRscTexture->GetTexture()->Unbind();


	// Default Mesh Material...
	{
		Ptr<MaterialShader> matShader( new MaterialShader() );
		matShader->SetName("Default_Mesh");
		matShader->SetDomain(ERenderShaderDomain::Mesh);
		matShader->SetType(ERenderShaderType::Opaque);
		matShader->SetMaterialFunction("shaders/Materials/DefaultMaterial.glsl");
		matShader->AddSampler("inCheckerTexture");
		matShader->LoadOnGpu();

		Ptr<Material> mat(new Material(matShader));
		mat->SetTexture("inCheckerTexture", checkerTexture.get());
		mat->LoadOnGpu();
		
		defaultMaterials.model = mat;
	}



	// Default Terrain Material...
	{
		Ptr<MaterialShader> matShader(new MaterialShader());
		matShader->SetName("Default_Terrain");
		matShader->SetDomain(ERenderShaderDomain::Terrain);
		matShader->SetType(ERenderShaderType::Opaque);
		matShader->SetMaterialFunction("shaders/Materials/TerrainMaterial.glsl");
		matShader->AddSampler("inCheckerTexture");
		matShader->LoadOnGpu();

		Ptr<Material> mat(new Material(matShader));
		mat->LoadOnGpu();

		defaultMaterials.terrain = mat;
	}
}






} // End of namespace Raven.

