#ifndef __H_RENDERBUFFER__
#define __H_RENDERBUFFER__

#include <glad/glad.h>

namespace yellowEngine
{
	class RenderBuffer
	{
		friend class FrameBuffer;
	public:
		RenderBuffer(GLenum internalFormat, int width, int height);
		~RenderBuffer();

		void bind();
		static void unbind();

	private:
		unsigned int _renderBufferHandle;
	};
}

#endif