

#include "GLBuffer.h"
#include "GL/glew.h"




using namespace Raven;





GLBuffer::GLBuffer()
	: id(0)
	, size(0)
	, type(EGLBufferType::None)
	, usage(EGLBufferUsage::StaticDraw)
{

}


GLBuffer::~GLBuffer()
{
	if (id != 0)
	{
		glDeleteBuffers(1, &id);
	}
}


GLBuffer* GLBuffer::Create(EGLBufferType type, int size, EGLBufferUsage usage)
{
	GLBuffer* buffer = new GLBuffer();
	buffer->usage = usage;


	glGenBuffers(1, &buffer->id);
	glBindBuffer((GLENUM)type, buffer->id);
	glBufferData((GLENUM)type, size, nullptr, (GLENUM)usage);
	glBindBuffer((GLENUM)type, 0);

	return buffer;
}


GLBuffer* GLBuffer::Create(EGLBufferType type, int size, void* data, EGLBufferUsage usage)
{
	GLBuffer* buffer = new GLBuffer();
	buffer->usage = usage;

	glGenBuffers(1, &buffer->id);
	glBindBuffer((GLENUM)type, buffer->id);
	glBufferData((GLENUM)type, size, data, (GLENUM)usage);
	glBindBuffer((GLENUM)type, 0);

	return buffer;
}


void GLBuffer::UpdateData(int dataSize, void* data)
{
	size = dataSize;

	glBindBuffer((GLENUM)type, id);
	glBufferData((GLENUM)type, size, data, (GLENUM)usage);
	glBindBuffer((GLENUM)type, 0);
}


void GLBuffer::UpdateSubData(int dataSize, int offset, void* data)
{
	// TODO:: Assert on data size exceeding the allocated size.
	// check(size <= dataSize)

	glBindBuffer((GLENUM)type, id);
	glBufferSubData((GLENUM)type, offset, dataSize, data);
	glBindBuffer((GLENUM)type, 0);
}


void GLBuffer::Bind()
{
	glBindBuffer((GLENUM)type, id);
}


void GLBuffer::Unbind()
{
	glBindBuffer((GLENUM)type, 0);
}


void GLBuffer::BindBase(int binding)
{
	glBindBufferBase((GLENUM)type, binding, id);
}


void GLBuffer::BindRange(int binding, int offset, int size)
{
	glBindBufferRange((GLENUM)type, binding, id, offset, size);
}
