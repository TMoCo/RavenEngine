#include "GLRenderBuffer.h"

#include "GL/glew.h"



using namespace Raven;



GLRenderBuffer::GLRenderBuffer()
	: id(0)
{

}


GLRenderBuffer::~GLRenderBuffer()
{
	if (id != 0)
	{
		glDeleteRenderbuffers(1, &id);
	}

}


GLRenderBuffer* GLRenderBuffer::Create(EGLTexture format, int width, int height)
{
	GLRenderBuffer* rd = new GLRenderBuffer();

	glGenRenderbuffers(1, &rd->id);

	return rd;
}


void GLRenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}


void GLRenderBuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


void GLRenderBuffer::UpdateStorage(EGLTexture format, int width, int height)
{
	glRenderbufferStorage(GL_RENDERBUFFER, (GLENUM)format, width, height);
}
