#include "GLFrameBuffer.h"
#include "GLTexture.h"
#include "GLRenderBuffer.h"

#include "Logger/Console.h"
#include "GL/glew.h"



using namespace Raven;




GLFrameBuffer::GLFrameBuffer()
	: id(0)
{

}


GLFrameBuffer::~GLFrameBuffer()
{
	if (id != 0)
	{
		glDeleteFramebuffers(1, &id);
	}
}


GLFrameBuffer* GLFrameBuffer::Create()
{
	GLFrameBuffer* fbo = new GLFrameBuffer();

	glGenFramebuffers(1, &fbo->id);

	return fbo;
}


void GLFrameBuffer::Attach(EGLAttachment attachment, int level, GLTexture* tex, int layer)
{
	GLAttachmentData data;
	data.level = level;
	data.layer = layer;
	data.texRef = tex;
	data.rdBufferRef = nullptr;

	attachments[attachment] = data;
}


void GLFrameBuffer::Attach(EGLAttachment attachment, GLRenderBuffer* tex)
{
	GLAttachmentData data;
	data.level = 0;
	data.layer = 0;
	data.texRef = nullptr;
	data.rdBufferRef = tex;


	attachments[attachment] = data;
}


void GLFrameBuffer::ClearAttachment(EGLAttachment attachment)
{
	attachments.erase(attachment);
}


void GLFrameBuffer::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);


	// Attachments...
	for (const auto& data : attachments)
	{
		const auto& att = data.second;

		// Texture Attachment?
		if (att.texRef)
		{
			EGLTexture texType = att.texRef->GetType();
			if (texType == EGLTexture::Texture2D)
			{
				// Attach Texture 2D.
				glFramebufferTexture2D(
					GL_FRAMEBUFFER,
					(GLENUM)att.target, 
					GL_TEXTURE_2D, 
					att.texRef->GetID(),
					att.level
				);

			}
			else if (texType == EGLTexture::CubeMap)
			{
				// Attach CubeMap layer.
				glFramebufferTexture2D(
					GL_FRAMEBUFFER,
					(GLENUM)att.target,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + att.layer,
					att.texRef->GetID(),
					att.level
				);

			}
			else
			{
				LOGW("GLFrameBuffer - Trying to attach unsupported texture type.");
			}
		}
		else if (att.rdBufferRef) // Render buffer attachment?
		{
			// Attach Render Buffer.
			glFramebufferRenderbuffer(
				GL_FRAMEBUFFER,
				(GLENUM)att.target,
				GL_RENDERBUFFER,
				att.rdBufferRef->GetID() 
			);

		}

	}

	// Validate Completion of Framebuffer.
	ValidateStatus();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GLFrameBuffer::ValidateStatus()
{
	GLENUM fbostatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	// Complete?
	if (fbostatus== GL_FRAMEBUFFER_COMPLETE)
		return;

	switch (fbostatus)
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		LOGW("GLFrameBuffer Status UNDEFINED.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		LOGW("GLFrameBuffer Status INCOMPLETE_ATTACHMENT.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		LOGW("GLFrameBuffer Status INCOMPLETE_MISSING_ATTACHMENT.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		LOGW("GLFrameBuffer Status INCOMPLETE_DRAW_BUFFER.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		LOGW("GLFrameBuffer Status INCOMPLETE_READ_BUFFER.");
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		LOGW("GLFrameBuffer Status UNSUPPORTED.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		LOGW("GLFrameBuffer Status INCOMPLETE_MULTISAMPLE.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		LOGW("GLFrameBuffer Status INCOMPLETE_LAYER_TARGETS.");
		break;
	}
}
