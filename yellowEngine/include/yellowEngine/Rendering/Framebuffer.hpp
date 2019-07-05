#ifndef __H_FRAMEBUFFER__
#define __H_FRAMEBUFFER__

#include <vector>
#include <glad/glad.h>

#include "yellowEngine/Rendering/Texture.hpp"
#include "yellowEngine/Rendering/RenderBuffer.hpp"

namespace yellowEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void addColorAttachment(const char* usage, int internalFormat, int width, int height, int format, GLenum type);
		void addDepthAttachment(int width, int height);
		void addDepthTexture(const char* uniqueName, int width, int height);
		void addDepthStencilAttachment(int width, int height);
		void init();

		void bind();
		static void unbind();
		void bindForDrawing();
		void bindForReading();
		void setDrawBuffer(int index, int count = 1);
		void setReadBuffer(int index);

		const std::vector<std::pair<std::string, Texture*>>& getColorBuffers();
		Texture* getShadowTexture() { return _depthTexture; }

	protected:
		unsigned int _frameBufferHandle;

		std::vector<std::pair<std::string, Texture*>> _colorBuffers;
		Texture* _depthTexture;
		RenderBuffer* _depthBuffer;
		RenderBuffer* _depthStencilBuffer;
	};
}

#endif