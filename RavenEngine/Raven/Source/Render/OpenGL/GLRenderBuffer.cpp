#include "GLRenderBuffer.h"

#include "GL/glew.h"



namespace Raven {



GLRenderBuffer::GLRenderBuffer()
	: id(0)
	, format(EGLFormat::None)
{

}


GLRenderBuffer::~GLRenderBuffer()
{
	if (id != 0)
	{
		glDeleteRenderbuffers(1, &id);
	}

}


GLRenderBuffer* GLRenderBuffer::Create(EGLFormat format, int width, int height)
{
	GLRenderBuffer* rd = new GLRenderBuffer();
	rd->format = format;
	rd->width = width;
	rd->height = height;

	glGenRenderbuffers(1, &rd->id);
	glBindRenderbuffer(GL_RENDERBUFFER, rd->id);
	glRenderbufferStorage(GL_RENDERBUFFER, (GLENUM)format, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

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


void GLRenderBuffer::UpdateStorage(EGLFormat format, int width, int height)
{
	glRenderbufferStorage(GL_RENDERBUFFER, (GLENUM)format, width, height);
}



} // End of namespace Raven.
