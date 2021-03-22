#include "RenderRscTexture.h"
#include "Render/OpenGL/GLTexture.h"



#include "Utilities/Core.h"



namespace Raven {



RenderRscTexture::RenderRscTexture()
{

}


RenderRscTexture::~RenderRscTexture()
{
	delete tex;
}


void RenderRscTexture::Load(EGLTexture type, EGLFormat format, const glm::ivec2& size, void* data)
{
	tex = GLTexture::Create(type, format);
	
	switch (type)
	{
	case EGLTexture::Texture2D:
	{
		tex->Bind();
		tex->UpdateTexData(0, size.x, size.y, data);
		tex->UpdateTexParams();
		tex->Unbind();
	}
			break;

			 
	default: // Not Supported...
		RAVEN_ASSERT(0, "RenderRscTexture - Texture type not supported.");
		break;
	}


}





} // End of namespace Raven.


