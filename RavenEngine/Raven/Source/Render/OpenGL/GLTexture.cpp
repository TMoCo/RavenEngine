#include "GLTexture.h"

#include "GL/glew.h"



using namespace Raven;





GLTexture::GLTexture()
	: id(0)
	, type(EGLTexture::None)
	, format(EGLFormat::None)
	, filter(EGLFilter::Nearest)
	, wrap(EGLWrap::ClampToEdge)
{

}


GLTexture::~GLTexture()
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
	}
}


GLTexture* GLTexture::Create(EGLTexture type, EGLFormat format)
{
	GLTexture* tex = new GLTexture();

	tex->type = type;
	tex->format = format;
	tex->width = 0;
	tex->height = 0;
	glGenTextures(1, &tex->id);

	return tex;
}


GLTexture* GLTexture::Create2D(EGLFormat format, int width, int height, EGLFilter filter, EGLWrap wrap)
{
	GLTexture* tex = new GLTexture();

	tex->type = EGLTexture::Texture2D;
	tex->format = format;
	tex->width = width;
	tex->height = height;
	tex->filter = filter;
	tex->wrap = wrap;
	glGenTextures(1, &tex->id);

	tex->Bind();
	tex->UpdateTexData(0, width, height, nullptr);
	tex->UpdateTexParams();
	tex->Unbind();

	return tex;
}


GLTexture* GLTexture::Create2D(EGLFormat format, int width, int height, const void* data, EGLFilter filter, EGLWrap wrap)
{
	GLTexture* tex = new GLTexture();

	tex->type = EGLTexture::Texture2D;
	tex->format = format;
	tex->width = width;
	tex->height = height;
	tex->filter = filter;
	tex->wrap = wrap;
	glGenTextures(1, &tex->id);

	tex->Bind();
	tex->UpdateTexData(0, width, height, data);
	tex->UpdateTexParams();
	tex->Unbind();

	return tex;
}


void GLTexture::UpdateTexData(int level, int newWidth, int newHeight, const void* data)
{
	width = newWidth;
	height = newHeight;

	// Get pixel information about based on the current format
	GLENUM pixelFormat;
	GLENUM pixelType;
	int alignment = 4;
	GetPixelInfo(format, pixelFormat, pixelType, alignment);

	// Pixel Data Alignment...
	if (alignment != 4)
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);


	// Texture Data...
	glTexImage2D((GLENUM)type, level, (GLENUM)format, width, height, 0, pixelFormat, pixelType, data);

	
	// Reset alignment back to default.
	if (alignment != 4)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

}


void GLTexture::UpdateTexData(int level, int newWidth, int newHeight, int numLayers, const void* data)
{
	width = newWidth;
	height = newHeight;

	// Get pixel information about based on the current format
	GLENUM pixelFormat;
	GLENUM pixelType;
	int alignment = 4;
	GetPixelInfo(format, pixelFormat, pixelType, alignment);

	// Pixel Data Alignment...
	if (alignment != 4)
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);


	// For now only cubemap...
	if (type == EGLTexture::CubeMap)
	{
		for (int i = 0; i < numLayers; ++i)
		{
			// Texture Data...
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, (GLENUM)format, width, height, 0, pixelFormat, pixelType, data);
		}
	}



	// Reset alignment back to default.
	if (alignment != 4)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}


void GLTexture::UpdateTexParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLINT)filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLINT)filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLINT)wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLINT)wrap);
}


void GLTexture::GenerateMipmaps()
{
	glGenerateMipmap((GLENUM)type);
}


void GLTexture::Bind()
{
	glBindTexture((GLENUM)type, id);
}


void GLTexture::Unbind()
{
	glBindTexture((GLENUM)type, 0);
}


void GLTexture::GetPixelInfo(EGLFormat format, GLENUM& pixelFormat, GLENUM& pixelType, int& alignment)
{
	alignment = 1;

	switch (format)
	{
	case EGLFormat::Depth24:
	case EGLFormat::Depth32:
		pixelFormat = GL_DEPTH_COMPONENT;
		pixelType = GL_FLOAT;
		break;

	case EGLFormat::DepthStencil:
		pixelFormat = GL_DEPTH_STENCIL;
		pixelType = GL_UNSIGNED_INT_24_8;
		break;

	case EGLFormat::R16F:
		pixelFormat = GL_R;
		pixelType = GL_FLOAT;
		break;

	case EGLFormat::RGB16F:
		pixelFormat = GL_RGB;
		pixelType = GL_FLOAT;
		break;

	case EGLFormat::RGBA16F:
		pixelFormat = GL_RGBA;
		pixelType = GL_FLOAT;
		alignment = 4;
		break;

	case EGLFormat::RGBA:
		pixelFormat = GL_RGBA;
		pixelType = GL_UNSIGNED_BYTE;
		alignment = 4;
		break;

	default:
		pixelFormat = (GLENUM)format;
		pixelType = GL_UNSIGNED_BYTE;
		break;
	}

}


void GLTexture::Active(int i)
{
	glBindTexture((GLENUM)type, id);
	glActiveTexture(GL_TEXTURE0 + i);
}


void GLTexture::SetFilter(EGLFilter value)
{
	filter = value;
}


void GLTexture::SetWrap(EGLWrap value)
{
	wrap = value;
}
