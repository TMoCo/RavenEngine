#include "RenderTarget.h"
#include "OpenGL/GLFrameBuffer.h"
#include "OpenGL/GLRenderBuffer.h"
#include "OpenGL/GLTexture.h"



namespace Raven {




RenderTarget::RenderTarget(EGLTexture type, EGLFormat format)
	: fbo(nullptr)
	, rtDepth(nullptr)
	, dirty(true)
{
	RAVEN_ASSERT(type == EGLTexture::Texture2D, "NOT SUPPORTED YET....");
	rtTexture = GLTexture::Create(type, format);
}


RenderTarget::~RenderTarget()
{
	delete rtTexture;
	delete rtDepth;
	delete fbo;
}


uint32_t RenderTarget::GetRTTextureID()
{
	return rtTexture->GetID();
}


void RenderTarget::Resize(glm::ivec2 newSize)
{
	dirty = newSize.x > size.x || newSize.y > size.y;;
	size = newSize;
}


void RenderTarget::SetClearColor(const glm::vec4& color)
{
	clearColor = color;
}


void RenderTarget::SetSampleProeprties(EGLFilter filter, EGLWrap wrap)
{
	rtTexture->SetFilter(filter);
	rtTexture->SetWrap(wrap);
}


void RenderTarget::Update()
{
	dirty = false;

	// Update Texture...
	bool rtResize = false;
	rtTexture->Bind();

	// Need Resize?
	if ( size.x > rtTexture->GetWidth() 
		|| size.y > rtTexture->GetHeight() )
	{
		rtTexture->UpdateTexData(0, size.x, size.y, nullptr);
		rtResize = true;
	}

	rtTexture->UpdateTexParams();
	rtTexture->Unbind();


	// Is Target Resized?
	if (rtResize)
	{
		// Not Created?
		if (!rtDepth)
		{
			rtDepth = GLRenderBuffer::Create(EGLFormat::Depth32, size.x, size.y);
		}
		else
		{
			// Update Depth Target.
			rtDepth->Bind();
			rtDepth->UpdateStorage(EGLFormat::Depth32, size.x, size.y);
			rtDepth->Unbind();
		}
	}


	// Framebuffer...
	if (!fbo)
	{
		fbo = GLFrameBuffer::Create();
		fbo->Attach(EGLAttachment::Color0, 0, rtTexture, 0);

		// Attach Depth.
		if (rtDepth)
			fbo->Attach(EGLAttachment::Depth, rtDepth);

		fbo->Update();
	}

}


glm::ivec2 RenderTarget::GetActualSize() const
{
	return glm::ivec2(rtTexture->GetWidth(), rtTexture->GetHeight());
}


} // End of namespace Raven.
