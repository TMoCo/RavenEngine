#include "RenderRscTexture.h"
#include "Render/OpenGL/GLTexture.h"



#include "Utilities/Core.h"



namespace Raven {



RenderRscTexture::RenderRscTexture()
{

}


RenderRscTexture::~RenderRscTexture()
{

}


void RenderRscTexture::Load(EGLTexture type, EGLFormat format, const glm::ivec2& size, void* data)
{
	texture = Ptr<GLTexture>( GLTexture::Create(type, format) );
	
	switch (type)
	{
	case EGLTexture::Texture2D:
	{
		texture->Bind();
		texture->UpdateTexData(0, size.x, size.y, data);
		texture->UpdateTexParams();
		texture->Unbind();
	}
			break;

	case EGLTexture::CubeMap:
	{
		texture->Bind();

		for (int32_t f = 0; f < 6; ++f)
		{
			uint8_t* faceData = (uint8_t*)data;

			// Not Null?
			if (data)
			{
				faceData += f * size.x * size.y * (GLTexture::GetBPP(format) >> 3);
			}

			texture->UpdateTexData(0, size.x, size.y, f, faceData);
		}

		texture->UpdateTexParams();
		texture->Unbind();
	}
		break;

	default: // Not Supported...
		RAVEN_ASSERT(0, "RenderRscTexture - Texture type not supported.");
		break;
	}


}





} // End of namespace Raven.


