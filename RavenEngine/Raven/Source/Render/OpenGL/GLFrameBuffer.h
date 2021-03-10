#pragma once




#include "GLTypes.h"

#include <map>





namespace Raven
{
	//
	class GLTexture;
	class GLRenderBuffer;




	// Represent a single attachemnt fo framebuffer.
	struct GLAttachmentData
	{
		// The attachment 
		EGLAttachment target;

		// The texture that is attachted.
		GLTexture* texRef;

		// The render buffer that is attachted.
		GLRenderBuffer* rdBufferRef;

		// The texture lod level.
		int level;

		// If the texture has multipe layer, this is the index of 
		// the layer we want to attach
		int layer;
	};




	//
	//
	class GLFrameBuffer
	{
	private:
		// Construct. 
		GLFrameBuffer();

	public:
		// Destruct.
		~GLFrameBuffer();

	  // Return the opengl id of the buffer.
		inline GLUINT GetID() const { return id; }

		// Create the framebuffer.
		static GLFrameBuffer* Create();

		// Attacment texture to a framebuffer attachment.
		//
		void Attach(EGLAttachment attachment, int level, GLTexture* tex, int layer = 0);

		// Attacment renderbuffer to a framebuffer attachment.
		//
		void Attach(EGLAttachment attachment, GLRenderBuffer* tex);

		// Clear an attachment.
		void ClearAttachment(EGLAttachment attachment);

		// Update the OpenGL framebuffer and its attachments.
		void Update();

	private:
		// Validate framebuffer status.
		void ValidateStatus();

	private:
		// OpenGL Object ID.
		GLUINT id;

		// Framebuffer attachments.
		std::map<EGLAttachment, GLAttachmentData> attachments;
	};

}



