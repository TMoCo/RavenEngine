#pragma once




#include "Utilities/Core.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


#include "OpenGL/GLTypes.h"







namespace Raven
{
	class GLTexture;
	class GLRenderBuffer;
	class GLFrameBuffer;
	class RenderModule;



	// RenderTarget:
	//	 - a Target for our rendering, we render the scene to.
	//
	class RenderTarget
	{
		// Friend...
		friend class RenderModule;

	public:
		// Construct.
		RenderTarget(EGLTexture type, EGLFormat format);

		// Destruct. 
		~RenderTarget();

		// Return the OpenGL ID of the render target texture.
		uint32_t GetRTTextureID();

		// Resize the render target. If the size is smaller than the currernt one, it will only resize the
		// rendering area but will leave the render texture actual size unchanged.
		void Resize(glm::ivec2 newSize);

		// Set the clear color for rendering.
		void SetClearColor(const glm::vec4& color);

		// If true the render target data need to be updated.
		inline bool IsDirty() { return dirty; }

		// Set render target sample properties. 
		void SetSampleProeprties(EGLFilter filter, EGLWrap wrap);

		// Return the size of the render target.
		inline const glm::ivec2& GetSize() const { return size; }

		// Return the actual size of the render target texture.
	  glm::ivec2 GetActualSize() const;

		// Return the aspect ratio of the render target size.
		inline float GetAspectRatio() { return (float)size.x / (float)size.y; }

		// Return the clear color used to clear the render target.
		inline const glm::vec4& GetClearColor() { return clearColor; }

	private:
		// Update the target to match the new data.
		void Update();

		/** Return Framebuffer. */
		inline GLFrameBuffer* GetFBO() { return fbo; }

	private:
		// Render Target Texture.
		GLTexture* rtTexture;

		// Render Depth.
		GLRenderBuffer* rtDepth;

		// Framebuffer Object.
		GLFrameBuffer* fbo;

		// The render size of the target.
		glm::ivec2 size;

		// The clear color to use when clearing the render texture.
		glm::vec4 clearColor;

		// Flag used to know when the render target is dirty and need updating.
		bool dirty;

	};
}

