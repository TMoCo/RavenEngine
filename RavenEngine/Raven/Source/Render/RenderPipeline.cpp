#include "RenderPipeline.h"

#include "RenderTarget.h"
#include "RenderObjects/RenderScene.h"
#include "RenderObjects/RenderScreen.h"
#include "RenderObjects/RenderPass.h"
#include "RenderResource/Shader/UniformBuffer.h"
#include "RenderResource/Shader/RenderRscShader.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLShader.h"
#include "OpenGL/GLFrameBuffer.h"



#include "GL/glew.h"




namespace Raven {



RenderPipeline::RenderPipeline()
	: rtarget(nullptr)
	, rscene(nullptr)
{

}


RenderPipeline::~RenderPipeline()
{

}


void RenderPipeline::Initialize()
{
	// Create Render Screen
	rscreen = Ptr<RenderScreen>( RenderScreen::Create() );

	// Setup...
	SetupRenderPasses();
	SetupShaders();

	// Uniforms...
	uniforms.common = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::CommonBlock, true) );
	uniforms.light = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::LightingBlock, true) );
	
}


void RenderPipeline::Destroy()
{
	rscreen.reset();
	uniforms.common.reset();
	uniforms.light.reset();


}


void RenderPipeline::Begin(RenderTarget* target, RenderScene* scene, float time)
{
	RAVEN_ASSERT(rtarget == nullptr && rscene == nullptr, "Can't Begin a new pipeline render while the previous one is not done.");
	rtarget = target;
	rscene = scene;

	// The Viewport
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.z = (float)rtarget->GetSize().x;
	viewport.w = (float)rtarget->GetSize().y;

	// Render Target Clear Color
	gbuffer->SetClearColor(0, rtarget->GetClearColor());


	// Check render passe target size, and resize it if needed.
	if (gbuffer->GetSize().x < viewport.z || gbuffer->GetSize().y < viewport.w)
	{
		glm::ivec2 newSize(viewport.z, viewport.w);
		Resize(newSize);
	}


	// Update Common Uniform Buffer...
	uniforms.common->SetDataValue(0, rscene->GetViewProjection());
	uniforms.common->SetDataValue(1, rscene->GetViewProjectionInverse());
	uniforms.common->SetDataValue(2, rscene->GetViewDir());
	uniforms.common->SetDataValue(3, rscene->GetViewPos());
	uniforms.common->SetDataValue(4, viewport);
	uniforms.common->SetDataValue(5, glm::vec4(rscene->GetEnvironment().sunDir, 0.0f));
	uniforms.common->SetDataValue(6, glm::vec4(rscene->GetEnvironment().sunColor, rscene->GetEnvironment().sunPower));
	uniforms.common->SetDataValue(7, glm::vec2(rscene->GetNear(), rscene->GetFar()));
	uniforms.common->SetDataValue(8, time);
	uniforms.common->Update();
	uniforms.common->BindBase();

}


void RenderPipeline::End()
{
	RAVEN_ASSERT(rtarget != nullptr && rscene != nullptr, "Can't End a pipeline render without starting one.");

	//
	FBBlitViewport blitViewport;
	blitViewport.x0 = (int)viewport.x;
	blitViewport.y0 = (int)viewport.y;
	blitViewport.x1 = (int)viewport.z;
	blitViewport.y1 = (int)viewport.w;

	// Blit...
	lighting->GetFBO()->Blit(
		rtarget->GetFBO(),
		EGLAttachment::Color0,
		EGLAttachment::Color0,
		EGLBufferMask::Color,
		EGLFilter::Nearest,
		blitViewport,
		blitViewport
	);


	rtarget = nullptr;
	rscene = nullptr;
}


void RenderPipeline::Render()
{
	RAVEN_ASSERT(rtarget != nullptr && rscene != nullptr, "Can't render a pipeline without starting one.");

	// Global OpenGL States...
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 
	// Deferred: G-Buffer Pass
	{
		gbuffer->Begin(viewport, true);

		// Draw the opaque part of the scene to the G-Buffer
		rscene->Draw(ERSceneBatch::Opaque);

		// Draw debug
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			rscene->Draw(ERSceneBatch::Debug);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}



	// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 
	// Deferred: Lighting Pass
	{
		GLShader* shader = lightingShader->GetShader();
		lighting->Begin(viewport, false);

		// Set G-Buffer Textures
		shader->Use();
		gbuffer->GetTexture(0)->Active(0); // Albedo + Specular.
		gbuffer->GetTexture(1)->Active(1); // Normal.
		gbuffer->GetTexture(2)->Active(2); // BRDF.
		gbuffer->GetDepthTexture()->Active(3); // Depth.

		//
		rscreen->SetRTSize( gbuffer->GetSize() );
		rscreen->Draw(shader);
	}




}


void RenderPipeline::Resize(const glm::ivec2& newSize)
{
	gbuffer->Resize(newSize);
	lighting->Resize(newSize);
	//forward->Resize(newSize);
}


void RenderPipeline::SetupRenderPasses()
{
	glm::ivec2 size(128, 128);

	// G-Buffer Rende Pass
	{
		// Albedo + Specular
		Ptr<GLTexture> albedoSpecularTarget0 = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::RGBA,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));

		// Normal
		Ptr<GLTexture> normalTarget1 = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::RGB16F,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));

		// BRDF: Roughness + Metallic + ??
		Ptr<GLTexture> brdfTarget2 = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::RGBA,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));

		// Depth
		Ptr<GLTexture> depthTarget = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::Depth32,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));


		gbuffer = Ptr<RenderPass>(new RenderPass());
		gbuffer->AddTexture(0, albedoSpecularTarget0);
		gbuffer->AddTexture(1, normalTarget1);
		gbuffer->AddTexture(2, brdfTarget2);
		gbuffer->SetDepthTexture(depthTarget, false);
		gbuffer->SetSize(size);
		gbuffer->Build();
	}


	// Lighting Rende Pass
	{
		// normal
		Ptr<GLTexture> lightingTexture = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::RGBA,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));


		lighting = Ptr<RenderPass>(new RenderPass());
		lighting->AddTexture(0, lightingTexture);
		lighting->SetSize(size);
		lighting->Build();
	}

}


void RenderPipeline::SetupShaders()
{
	// Lighting Shader Pass...
	{
		RenderRscShaderDomainCreateData shaderDomainData;
		shaderDomainData.AddSource(EGLShaderStage::Vertex, "shaders/ScreenTriangleVert.glsl");
		shaderDomainData.AddSource(EGLShaderStage::Fragment, "shaders/DeferredLighting.glsl");

		// Shader Type Data
		RenderRscShaderCreateData shaderData;
		shaderData.type = ERenderShaderType::Opaque;
		shaderData.name = "DeferredLighting_Shader";

		lightingShader = Ptr<RenderRscShader>( RenderRscShader::CreateCustom(shaderDomainData, shaderData) );
		lightingShader->GetInput().AddBlockInput(RenderShaderInput::CommonBlock);
		lightingShader->GetInput().AddSamplerInput("inAlbedoSpecular");
		lightingShader->GetInput().AddSamplerInput("inNormal");
		lightingShader->GetInput().AddSamplerInput("inBRDF");
		lightingShader->GetInput().AddSamplerInput("inDepth");
		lightingShader->BindBlockInputs();
		lightingShader->BindSamplers();
	}
}




} // End of namespace Raven.

