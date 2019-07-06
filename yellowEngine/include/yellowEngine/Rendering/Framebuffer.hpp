#ifndef __H_FRAMEBUFFER__
#define __H_FRAMEBUFFER__

#include <vector>
#include <glad/glad.h>

#include "yellowEngine/Rendering/Texture.hpp"
#include "yellowEngine/Rendering/RenderBuffer.hpp"
#include "yellowEngine/Rendering/CubeMap.hpp"

namespace yellowEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void addColorAttachment(const char* usage, int internalFormat, int width, int height, int format, GLenum type);
		void addDepthAttachment(int width, int height);
		void addDepthTexture(const char* name, int width, int height);
		void addDepthStencilAttachment(int width, int height);
		void addDepthCubeMap(int internalFormat, int width, int format, int type);
		void init();

		GLuint getDepthMapId();
		int getDepthMapWidth() { return _depthMapWidth; };
		int getDepthMapHeight() { return _depthMapHeight; };

		void bind();
		static void unbind();
		void bindForDrawing();
		void bindForReading();
		void setDrawBuffer(int index, int count = 1);
		void setReadBuffer(int index);

		unsigned int  getFrameBufferHandle() { return _frameBufferHandle; };
		const std::vector<std::pair<std::string, Texture*>>& getColorBuffers();
		void bindDepthTexture();

	protected:
		unsigned int _frameBufferHandle;

		int _depthMapHeight;
		int _depthMapWidth;

		// TODO:: abtract attachments
		std::vector<std::pair<std::string, Texture*>> _colorBuffers;
		Texture* _depthTexture;
		RenderBuffer* _depthBuffer;
		RenderBuffer* _depthStencilBuffer;
		CubeMap* _depthCubeMap;
	};
}

#endif