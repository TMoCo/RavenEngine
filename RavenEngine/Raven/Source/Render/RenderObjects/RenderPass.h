#pragma once




#include "Utilities/Core.h"
#include "Render/OpenGL/GLTypes.h"

#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include <vector>





namespace Raven
{
	class GLFrameBuffer;
	class GLTexture;
	class GLRenderBuffer;




	// Targets used by the render pass to draw on.
	struct RenderPassTarget
	{
		// Mip Level
		int32_t level;

		// Texture Layer, for example CubeMap layer
		int32_t layer;

		// Texture
		Ptr<GLTexture> texture;

		// The color used to clear the target.
		glm::vec4 clearValue;
	};






	//	RenderPass:
	//			- 
	class RenderPass
	{
	public:
		// Construct.
		RenderPass();

		// Destruct.
		~RenderPass();

		// Set the render pass render size.
		inline void SetSize(const glm::ivec2& inSize) { size = inSize; }
		inline void SetSize(int32_t x, int32_t y) { size = glm::ivec2(x, y); }

		// Return the render size of this render pass.
		inline const glm::ivec2& GetSize() const { return size; }

		// Add texture to the render pass at location index, each index is mapped
		// to a color attachment.
		void AddTexture(uint32_t index, const Ptr<GLTexture>& inTex, int32_t level = 0, int32_t layer = 0);

		// Set the clear color for a target.
		void SetClearColor(uint32_t index, const glm::vec4& color);

		// Set depth as a texture.
		void SetDepthTexture(const Ptr<GLTexture>& inTex, bool isWithStencil, int32_t level = 0, int32_t layer = 0);

		// Return a texture at location index.
		inline const Ptr<GLTexture>& GetTexture(int32_t index) { return textures[index].texture; }

		// Return the depth texture.
		inline const Ptr<GLTexture>& GetDepthTexture() { return depthTexture.texture; }

		// Set render buffer as color0 target.
		void SetRenderBufferForColor(bool val);

		// Set render buffer as depth target.
		void SetRenderBufferForDepth(bool val, bool isWithStencil);

		// Build this render pass by setting up the framebuffer and its attachments.
		void Build();

		// Destroy the render pass objects.
		void Destroy();

		// Bind RenderPass/Framebuffer.
		inline GLFrameBuffer* GetFBO() const { return fbo.get(); }

		// Resize all the render pass targets to a new size.
		void Resize(const glm::ivec2& newSize);

		// Begin the render pass.
		void Begin(const glm::ivec4& viewport, bool isClear);

	private:
		// Framebuffer of the render pass.
		Ptr<GLFrameBuffer> fbo;

		// The Color Target Texture, each index is mapped to a color attachment.
		std::vector<RenderPassTarget> textures;

		// The Depth Target Texture.
		RenderPassTarget depthTexture;

		// If true then the depth texture is DEPTH_STENCIL
		bool isStencilDepth;

		// Render Buffers if requested 0:Color0, 1:Depth.
		Ptr<GLRenderBuffer> renderBuffers[2];

		// If true use render buffer for as color0 attachment.
		bool isColorRenderBuffer;

		// If true use render buffer as depth attachment.
		bool isDepthRenderBuffer;

		// If true the render pass is built and ready to use.
		bool isBuilt;

		// the width and height of the render.
		glm::ivec2 size;
	};

}

