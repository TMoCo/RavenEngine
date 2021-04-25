#include "RenderTexFilter.h"

#include "RenderObjects/RenderPass.h"
#include "RenderObjects/RenderScreen.h"
#include "RenderObjects/RenderSphere.h"
#include "RenderResource/RenderRscTexture.h"
#include "RenderResource/Shader/RenderRscShader.h"


#include "OpenGL/GLTexture.h"
#include "OpenGL/GLShader.h"

#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"




namespace Raven {



static std::pair<glm::vec3, glm::vec3> cubeMapViews[6] = {
	// +/- X_AXIS
	std::make_pair( glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f) ),
	std::make_pair( glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f) ),

	// +/- Y_AXIS
	std::make_pair( glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) ),
	std::make_pair( glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f) ),

	// +/- Z_AXIS
	std::make_pair( glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f) ),
	std::make_pair( glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f) ),
};




// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 





RenderTexFilter::RenderTexFilter(Ptr<RenderScreen> screen, Ptr<RenderSphere> sphere)
	: rscreen(screen)
	, rsphere(sphere)
{

}


RenderTexFilter::~RenderTexFilter()
{

}


void RenderTexFilter::Initialize()
{
	// Create Filter Shadrs...
	CreateCubeMapGenShader();
	CreateIrradainceFilterShader();
	CreateSepcularFilterShader();
	CreateBRDFLUTShader();


	// Setup Filter RenderPass...
	filterPass = Ptr<RenderPass>(new RenderPass());

}


void RenderTexFilter::Destroy()
{
	cubeMapGenShader.reset();
	filterIrradianceShader.reset();
	filterSpecularShader[0].reset();
	filterSpecularShader[1].reset();
	brdfLUTShader.reset();
}


void RenderTexFilter::CreateCubeMapGenShader()
{
	RenderRscShaderDomainCreateData shaderDomainData;
	shaderDomainData.AddSource(EGLShaderStage::Vertex, "shaders/SphereVert.glsl");
	shaderDomainData.AddSource(EGLShaderStage::Fragment, "shaders/Filters/GenerateCubeMapFrag.glsl");

	// Shader Type Data
	RenderRscShaderCreateData shaderData;
	shaderData.type = ERenderShaderType::Opaque;
	shaderData.name = "GenerateCubeMap_Shader";

	cubeMapGenShader = Ptr<RenderRscShader>(RenderRscShader::CreateCustom(shaderDomainData, shaderData));
	cubeMapGenShader->GetInput().AddSamplerInput("inEquiTexture");
	cubeMapGenShader->BindSamplers();
}


void RenderTexFilter::CreateIrradainceFilterShader()
{
	RenderRscShaderDomainCreateData shaderDomainData;
	shaderDomainData.AddSource(EGLShaderStage::Vertex, "shaders/SphereVert.glsl");
	shaderDomainData.AddSource(EGLShaderStage::Fragment, "shaders/Filters/FilterIrradianceIBLFrag.glsl");
	shaderDomainData.AddImport(EGLShaderStageBit::FragmentBit, "shaders/CommonLight.glsl");

	// Shader Type Data
	RenderRscShaderCreateData shaderData;
	shaderData.type = ERenderShaderType::Opaque;
	shaderData.name = "FilterIrradianceIBL_Shader";

	filterIrradianceShader = Ptr<RenderRscShader>(RenderRscShader::CreateCustom(shaderDomainData, shaderData));
	filterIrradianceShader->GetInput().AddSamplerInput("inEnvTexture");
	filterIrradianceShader->BindSamplers();
}


void RenderTexFilter::CreateSepcularFilterShader()
{
	int32_t numSamples[2] = {
		512, 2048
	};

	for (int32_t i = 0; i < 2; ++i)
	{
		RenderRscShaderDomainCreateData shaderDomainData;
		shaderDomainData.AddSource(EGLShaderStage::Vertex, "shaders/SphereVert.glsl");
		shaderDomainData.AddSource(EGLShaderStage::Fragment, "shaders/Filters/FilterSpecularIBLFrag.glsl");
		shaderDomainData.AddImport(EGLShaderStageBit::FragmentBit, "shaders/CommonLight.glsl");
		shaderDomainData.AddPreprocessor("#define NUM_SAMPLES " + std::to_string(numSamples[i]));

		// Shader Type Data
		RenderRscShaderCreateData shaderData;
		shaderData.type = ERenderShaderType::Opaque;
		shaderData.name = "FilterSpecularIBL_Shader";

		filterSpecularShader[i] = Ptr<RenderRscShader>(RenderRscShader::CreateCustom(shaderDomainData, shaderData));
		filterSpecularShader[i]->GetInput().AddSamplerInput("inEnvTexture");
		filterSpecularShader[i]->BindSamplers();
	}
}


void RenderTexFilter::CreateBRDFLUTShader()
{
	RenderRscShaderDomainCreateData shaderDomainData;
	shaderDomainData.AddSource(EGLShaderStage::Vertex, "shaders/ScreenTriangleVert.glsl");
	shaderDomainData.AddSource(EGLShaderStage::Fragment, "shaders/Filters/GenerateBRDFLUTFrag.glsl");
	shaderDomainData.AddImport(EGLShaderStageBit::FragmentBit, "shaders/CommonLight.glsl");

	// Shader Type Data
	RenderRscShaderCreateData shaderData;
	shaderData.type = ERenderShaderType::Opaque;
	shaderData.name = "GenerateBRDF_Shader";

	brdfLUTShader = Ptr<RenderRscShader>(RenderRscShader::CreateCustom(shaderDomainData, shaderData));
	brdfLUTShader->BindSamplers();
}


void RenderTexFilter::GenCubeMap(RenderRscTexture* inTexture, RenderRscTexture* outCubeMap, bool isHDR)
{
	glm::ivec2 size(512, 512);
	outCubeMap->Load(EGLTexture::CubeMap, isHDR ? EGLFormat::RGB : EGLFormat::RGB16F, size, nullptr);
	outCubeMap->GetTexture()->Bind();
	outCubeMap->GetTexture()->SetWrap(EGLWrap::Mirror);
	outCubeMap->GetTexture()->SetFilter(EGLFilter::Linear);
	outCubeMap->GetTexture()->UpdateTexParams();
	outCubeMap->GetTexture()->Unbind();

	glm::ivec4 viewport(0, 0, size.x, size.y);
	filterPass->SetSize(size);

	//
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	GLShader* shader = cubeMapGenShader->GetShader();
	shader->Use();

	for (int32_t i = 0; i < 6; ++i)
	{
		filterPass->ReplaceTarget(0, outCubeMap->GetTexture(), 0, i);
		filterPass->Begin(viewport, false);

		//
		rsphere->SetProj(glm::perspective(glm::radians(90.0f), 1.0F, 0.01f, 10.0f));

		inTexture->GetTexture()->Active(0);
		rsphere->SetView(glm::lookAt(glm::vec3(0.0f), cubeMapViews[i].first, cubeMapViews[i].second));
		rsphere->Draw(shader);
	}

	RenderPass::End();
}


void RenderTexFilter::FilterIrradianceIBL(RenderRscTexture* inEnvTexture, RenderRscTexture* outIrradiance)
{
	glm::ivec2 size(64, 64);
	outIrradiance->Load(EGLTexture::CubeMap, EGLFormat::RGB, size, nullptr);

	//
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glm::ivec4 viewport(0, 0, size.x, size.y);
	filterPass->SetSize(size);

	GLShader* shader = filterIrradianceShader->GetShader();
	shader->Use();

	for (int32_t i = 0; i < 6; ++i)
	{
		filterPass->ReplaceTarget(0, outIrradiance->GetTexture(), 0, i);
		filterPass->Begin(viewport, false);

		inEnvTexture->GetTexture()->Active(0);
		rsphere->SetView(glm::lookAt(glm::vec3(0.0f), cubeMapViews[i].first, cubeMapViews[i].second));
		rsphere->Draw(shader);
	}

	RenderPass::End();
}



void RenderTexFilter::FilterSpecularIBL(RenderRscTexture* inEnvTexture, RenderRscTexture* outSpecular)
{
	int32_t numMips = 5;
	glm::ivec2 size(256, 256);
	outSpecular->Load(EGLTexture::CubeMap, EGLFormat::RGB, size, nullptr);
	outSpecular->GetTexture()->Bind();
	outSpecular->GetTexture()->SetWrap(EGLWrap::ClampToEdge);
	outSpecular->GetTexture()->SetFilter(EGLFilter::TriLinear);
	outSpecular->GetTexture()->SetMipLevels(0, numMips);
	outSpecular->GetTexture()->UpdateTexParams();
	outSpecular->GetTexture()->GenerateMipmaps();
	outSpecular->GetTexture()->Unbind();

	//
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	GLShader* shader = filterSpecularShader[1]->GetShader();
	shader->Use();

	//

	for (int32_t mip = 0; mip < numMips; ++mip)
	{
		float roughness = (float)mip / (float)(numMips - 1);
		shader->SetUniform("inRoughness", roughness);

		glm::ivec2 mipSize(size.x >> mip, size.y >> mip);
		glm::ivec4 viewport(0, 0, mipSize.x, mipSize.y);
		filterPass->SetSize(mipSize);

		for (int32_t i = 0; i < 6; ++i)
		{
			filterPass->ReplaceTarget(0, outSpecular->GetTexture(), mip, i);
			filterPass->Begin(viewport, false);

			inEnvTexture->GetTexture()->Active(0);
			rsphere->SetView(glm::lookAt(glm::vec3(0.0f), cubeMapViews[i].first, cubeMapViews[i].second));
			rsphere->Draw(shader);
		}
	}


	RenderPass::End();

}


void RenderTexFilter::GenBRDFLUT(RenderRscTexture* outBRDF)
{
	glm::ivec2 size(512, 512);
	outBRDF->Load(EGLTexture::Texture2D, EGLFormat::RG16F, size, nullptr);
	outBRDF->GetTexture()->Bind();
	outBRDF->GetTexture()->SetWrap(EGLWrap::ClampToEdge);
	outBRDF->GetTexture()->SetFilter(EGLFilter::Linear);
	outBRDF->GetTexture()->UpdateTexParams();
	outBRDF->GetTexture()->Unbind();


	//
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glm::ivec4 viewport(0, 0, size.x, size.y);
	filterPass->SetSize(size);

	GLShader* shader = brdfLUTShader->GetShader();
	shader->Use();

	filterPass->ReplaceTarget(0, outBRDF->GetTexture(), 0, 0);
	filterPass->Begin(viewport, true);

	rscreen->SetRTSize(glm::vec2(size.x, size.y));
	rscreen->Draw(shader);

	RenderPass::End();
}


} // End of namespace Raven.

