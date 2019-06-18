#ifndef __H_RENDERBUFFER__
#define __H_RENDERBUFFER__

#include <glad/glad.h>

namespace yellowEngine
{
	class RenderBuffer
	{
	public:
		RenderBuffer(int width, int height, GLenum format);
		~RenderBuffer();

		void bind();
		static void unbind();

	private:
		unsigned int _renderBufferHandle;
	};
}

#endif