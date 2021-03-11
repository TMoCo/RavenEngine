#include "RenderModule.h"


#include "Engine.h"

#include "OpenGL/GLContext.h"
#include "OpenGL/GLShader.h"
#include "OpenGL/GLFrameBuffer.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLRenderBuffer.h"
#include "OpenGL/GLTexture.h"

#include "RenderResource/RenderMeshResource.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtx/transform.hpp"


#include <chrono>



using namespace Raven;




// ~Testing.....
GLShader* phongShader = nullptr;
GLFrameBuffer* fbo = nullptr;
GLRenderBuffer* colorTarget = nullptr;
GLRenderBuffer* depthTarget = nullptr;

RenderMeshResource* meshRSC = nullptr;
std::vector<unsigned int> indices;
std::vector<glm::vec3> verts;

char* texData;
GLTexture* tex;

// ~Testing.....







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

	//..............................
	// ~Testing.....................


	// Build Testing Shader.
	phongShader = GLShader::Create("Phong_Shader");
	phongShader->SetSource(EGLShaderStage::Vertex, R"(

#version 330 core 

layout(location = 0) in vec3 inPositions;

uniform mat4 view;
uniform mat4 proj;

out vec2 uv;

void main()
{
	gl_Position = proj * view * vec4(inPositions, 1.0);
	uv = inPositions.xz * 0.5 + 0.5;
}

	)");


	phongShader->SetSource(EGLShaderStage::Fragment, R"(

#version 330 core

out vec4 fragColor;

in vec2 uv;
uniform float time;
uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, uv);
}

	)");

	phongShader->Build();


	// Build Testing Mesh.
	verts = {
		glm::vec3(-1.0f, 0.0f,-1.0f),
		glm::vec3( 1.0f, 0.0f,-1.0f),
		glm::vec3( 1.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),

	  glm::vec3(-1.0f, 1.4f,-1.0f),
		glm::vec3( 1.0f, 1.4f,-1.0f),
		glm::vec3( 1.0f, 1.4f, 1.0f),
		glm::vec3(-1.0f, 1.4f, 1.0f)
	};

	indices = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,
	};

	meshRSC = new RenderMeshResource();
	meshRSC->Load(verts, indices);



	// Framebuffer
	colorTarget = GLRenderBuffer::Create(EGLFormat::RGBA, 512, 512);
	depthTarget = GLRenderBuffer::Create(EGLFormat::Depth32, 512, 512);

	fbo = GLFrameBuffer::Create();
	fbo->Attach(EGLAttachment::Color0, colorTarget);
	fbo->Attach(EGLAttachment::Depth, depthTarget);
	fbo->Update();

	
	texData = new char[512 * 512 * 3];
	for (int y = 0; y < 512; ++y)
	{
		float fy = (float)y / 511.0f;

		for (int x = 0; x < 512; ++x)
		{
			float fx = (float)x / 511.0f;

			float nfx = fx * 2.0f - 1.0f;
			float nfy = fy * 2.0f - 1.0f;

			nfx *= 10.0f;
			nfy *= 15.0f;
			float r = glm::sqrt(nfx * nfx + nfy * nfy);
			r = glm::abs(glm::sin(r));

			r = r + 0.2f;

			texData[(x + y * 512) * 3 + 0] = (char)((1.0f - r) * 255.0f);
			texData[(x + y * 512) * 3 + 1] = (char)((1.0f - r) * 255.0f);
			texData[(x + y * 512) * 3 + 2] = (char)(r * 255.0f);
		}
	}

	tex = GLTexture::Create2D(EGLFormat::RGB, 512, 512, texData, EGLFilter::TriLinear, EGLWrap::Repeat);

	tex->Bind();
	tex->GenerateMipmaps();
	tex->Unbind();

	// ~Testing......
	//..............................



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
  //..............................
	// ~Testing.....................
  
	// Compute DeltaTime for each frame...
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime.time_since_epoch()).count();
	lastTime = currentTime;

	fbo->Bind(EGLFrameBuffer::Framebuffer);

	glViewport(0, 0, 512, 512);
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	static float i = 0;

	glm::vec3 eye = 
		glm::rotate(time, glm::vec3(0.0f, 1.0f, 0.0f)) 
		* glm::vec4(4.0f, glm::sin(time) * 9.0f, 4.0f, 1.0f);

	glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);

	phongShader->Use();
	phongShader->SetUniform("view", view);
	phongShader->SetUniform("proj", proj);

	phongShader->SetUniform("time", time);
	phongShader->SetUniform("tex", 0);

	tex->Active(0);

	meshRSC->GetArray()->Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	meshRSC->GetArray()->Unbind();

	fbo->Unbind(EGLFrameBuffer::Framebuffer);


	fbo->Blit(
		nullptr, // Default Framebuffer.
		EGLBufferMask::Color, // Color Mask Bit. 
		EGLFilter::Nearest, // Filter.
		FBBlitViewport(0, 0, 512, 512), // Src Viewport.
		FBBlitViewport(0, 0, 512, 512)  // Dst Viewport.
	);

	// ~Testing.....................
	//..............................

}


void RenderModule::EndRender()
{
	isRendering = false;

}
