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

		void addColorAttachment(int internalFormat, int width, int height, int format, GLenum type);
		void addDepthAttachment(int width, int height);
		void init();
		static void unbind();
		void bindForWriting();
		void bindForReading();
		void setBufferToRead(int index);

	protected:
		unsigned int _frameBufferHandle;
		std::vector<Texture*> _colorBuffers;
		RenderBuffer* _depthBuffer;
	};
}

#endif