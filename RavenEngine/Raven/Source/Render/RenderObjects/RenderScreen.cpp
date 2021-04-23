#include "RenderScreen.h"


#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLBuffer.h"
#include "Render/OpenGL/GLShader.h"


#include "GL/glew.h"



namespace Raven {



RenderScreen::RenderScreen()
	: rtSize(1.0f, 1.0f)
{

}


RenderScreen::~RenderScreen()
{
	delete vxArray;
	delete vxBuffer;
}


RenderScreen* RenderScreen::Create()
{
	RenderScreen* scr = new RenderScreen();

	// Screen Triangle.
	std::vector<glm::vec2> verts = {
		glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 0.0f),
		glm::vec2( 3.0f,-1.0f), glm::vec2(2.0f, 0.0f),
		glm::vec2(-1.0f, 3.0f), glm::vec2(0.0f, 2.0f),
	};

	scr->vxBuffer = GLBuffer::Create(
		EGLBufferType::Array,
		(int)(verts.size() * sizeof(glm::vec2)),
		verts.data(),
		EGLBufferUsage::StaticDraw
	);


	std::vector<GLVABuildAttribData> attributes{
		// Attribute 0 - Position
		{
			scr->vxBuffer,         // Buffers
			0,                     // Index
			2,                     // Type-Size
			EGLTypes::Float,       // Type
			sizeof(glm::vec2) * 2, // Stride
			0                      // offset
		},

		// Attribute 1 - Texture Coordinate
		{
			scr->vxBuffer,         // Buffers
			1,                     // Index
			2,                     // Type-Size
			EGLTypes::Float,       // Type
			sizeof(glm::vec2) * 2, // Stride
			sizeof(glm::vec2)      // offset
		}
	};


	scr->vxArray = GLVertexArray::Create();
	scr->vxArray->Build(attributes, nullptr);

	return scr;
}



void RenderScreen::Draw(GLShader* shader)
{
	shader->SetUniform("inRTSize", rtSize);
	vxArray->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}



} // End of namespace Raven.
