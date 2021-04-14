#include "RenderPipeline.h"

#include "RenderTarget.h"
#include "RenderObjects/RenderScene.h"
#include "RenderObjects/RenderLight.h"
#include "RenderObjects/RenderScreen.h"
#include "RenderObjects/RenderSphere.h"
#include "RenderObjects/RenderPass.h"
#include "RenderResource/Shader/UniformBuffer.h"
#include "RenderResource/Shader/RenderRscShader.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLShader.h"
#include "OpenGL/GLFrameBuffer.h"

#include "Engine.h"

#include "GL/glew.h"




namespace Raven {



RenderPipeline::RenderPipeline(Ptr<RenderScreen> screen, Ptr<RenderSphere> sphere)
	: rtarget(nullptr)
	, rscene(nullptr)
	, rscreen(screen)
	, rsphere(sphere)
{

}


RenderPipeline::~RenderPipeline()
{

}


void RenderPipeline::Initialize()
{
	// Setup...
	SetupRenderPasses();
	SetupShaders();

	// Uniforms...
	uniforms.common = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::CommonBlock, true) );
	uniforms.light_DEFERRED = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::LightingBlock_DEFERRED, true) );
	uniforms.light_FORWARD = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::LightingBlock_FORWARD, true) );
}


void RenderPipeline::Destroy()
{
	uniforms.common.reset();
	uniforms.light_DEFERRED.reset();
	uniforms.light_FORWARD.reset();


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
	glm::vec4 clearColor = rtarget->GetClearColor();
	clearColor.a = 0.0f;
	gbufferPass->SetClearColor(0, clearColor);


	// Check render passe target size, and resize it if needed.
	if (gbufferPass->GetSize().x < viewport.z || gbufferPass->GetSize().y < viewport.w)
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
	lightingPass->GetFBO()->Blit(
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
		gbufferPass->Begin(viewport, true);

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
		UpdateLights_DEFERRED();
		uniforms.light_DEFERRED->BindBase();

		GLShader* shader = lightingShader->GetShader();
		lightingPass->Begin(viewport, false);

		// Set G-Buffer Textures
		shader->Use();
		gbufferPass->GetTexture(0)->Active(0); // Albedo + Specular.
		gbufferPass->GetTexture(1)->Active(1); // Normal.
		gbufferPass->GetTexture(2)->Active(2); // BRDF.
		gbufferPass->GetDepthTexture()->Active(3); // Depth.

		// ~Testing----------------------------------------------------------------------------
		testEnv->Active(4);
		testBRDF->Active(5);
		// ~Testing----------------------------------------------------------------------------


		//
		rscreen->SetRTSize( gbufferPass->GetSize() );
		rscreen->Draw(shader);
	}



	// End...
	RenderPass::End();
}


void RenderPipeline::Resize(const glm::ivec2& newSize)
{
	gbufferPass->ResizeTargets(newSize);
	lightingPass->ResizeTargets(newSize);
	//forward->Resize(newSize);
}


void RenderPipeline::SetupRenderPasses()
{
	glm::ivec2 size(1920, 1080);

	// G-Buffer Rende Pass
	{
		// Albedo(RGB) + EmissionPower(B)
		Ptr<GLTexture> target0Albedo = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::RGBA,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));

		// Normal
		Ptr<GLTexture> target1Normal = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::RGB16F,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));

		// BRDF: Roughness(R) + Metallic(G) + Specular(B)
		Ptr<GLTexture> target2BRDF = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::RGBA,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));

		// Depth
		Ptr<GLTexture> targetDepth = Ptr<GLTexture>(GLTexture::Create2D(
			EGLFormat::Depth32,
			size.x, size.y,
			EGLFilter::Nearest,
			EGLWrap::ClampToEdge
		));


		gbufferPass = Ptr<RenderPass>(new RenderPass());
		
		// Targets...
		gbufferPass->AddTexture(0, target0Albedo);
		gbufferPass->AddTexture(1, target1Normal);
		gbufferPass->AddTexture(2, target2BRDF);
		gbufferPass->SetDepthTexture(targetDepth, false);
		gbufferPass->SetSize(size);
		gbufferPass->Build();
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


		lightingPass = Ptr<RenderPass>(new RenderPass());
		lightingPass->AddTexture(0, lightingTexture);
		lightingPass->SetSize(size);
		lightingPass->Build();
	}

}


void RenderPipeline::SetupShaders()
{
	// Lighting Shader Pass...
	{
		RenderRscShaderDomainCreateData shaderDomainData;
		shaderDomainData.AddSource(EGLShaderStage::Vertex, "shaders/ScreenTriangleVert.glsl");
		shaderDomainData.AddSource(EGLShaderStage::Fragment, "shaders/DeferredLighting.glsl");
		shaderDomainData.AddImport(EGLShaderStageBit::FragmentBit, "shaders/CommonLight.glsl");
		shaderDomainData.AddImport(EGLShaderStageBit::FragmentBit, "shaders/Lighting.glsl");
		shaderDomainData.AddPreprocessor("#define MAX_LIGHTS " + std::to_string(RENDER_PASS_DEFERRED_MAX_LIGHTS));
		shaderDomainData.AddPreprocessor("#define SCALE_UV_WITH_TARGET 1");

		// Shader Type Data
		RenderRscShaderCreateData shaderData;
		shaderData.type = ERenderShaderType::Opaque;
		shaderData.name = "DeferredLighting_Shader";

		lightingShader = Ptr<RenderRscShader>( RenderRscShader::CreateCustom(shaderDomainData, shaderData) );
		lightingShader->GetInput().AddBlockInput(RenderShaderInput::CommonBlock);
		lightingShader->GetInput().AddBlockInput(RenderShaderInput::LightingBlock_DEFERRED);
		lightingShader->GetInput().AddSamplerInput("inAlbedo");
		lightingShader->GetInput().AddSamplerInput("inNormal");
		lightingShader->GetInput().AddSamplerInput("inBRDF");
		lightingShader->GetInput().AddSamplerInput("inDepth");
		lightingShader->GetInput().AddSamplerInput("inSkyEnvironment");
		lightingShader->GetInput().AddSamplerInput("inEnvBRDF");
		lightingShader->BindBlockInputs();
		lightingShader->BindSamplers();
	}

}




void RenderPipeline::UpdateLights_DEFERRED()
{
	std::vector<glm::vec4> lightPos(RENDER_PASS_DEFERRED_MAX_LIGHTS);
	std::vector<glm::vec4> lightDir(RENDER_PASS_DEFERRED_MAX_LIGHTS);
	std::vector<glm::vec4> lightPower(RENDER_PASS_DEFERRED_MAX_LIGHTS);
	std::vector<glm::vec4> lightData(RENDER_PASS_DEFERRED_MAX_LIGHTS);


	for (int32_t i = 0; i < RENDER_PASS_DEFERRED_MAX_LIGHTS; ++i)
	{
		if (i >= rscene->GetLights().size())
		{
			lightData[i].r = 0.0f; // No Light.
			continue;
		}
		
		const auto& light =  rscene->GetLights()[i];
		lightPos[i] = glm::vec4(light->postion, 0.0f);
		lightDir[i] = glm::vec4(light->dir, 0.0f);
		lightData[i].r = light->type;
		lightData[i].g = light->innerAngle;
		lightData[i].b = light->outerAngle;
		lightData[i].a = light->radius;
		lightPower[i] = light->colorAndPower;
	}



	uniforms.light_DEFERRED->SetDataValues(0, lightDir);
	uniforms.light_DEFERRED->SetDataValues(1, lightPos);
	uniforms.light_DEFERRED->SetDataValues(2, lightPower);
	uniforms.light_DEFERRED->SetDataValues(3, lightData);
	uniforms.light_DEFERRED->Update();
}





} // End of namespace Raven.

