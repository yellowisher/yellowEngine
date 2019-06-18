#ifndef NULL
	#define NULL 0
#endif

#include "yellowEngine\Rendering\RenderBuffer.hpp"


namespace yellowEngine
{
	RenderBuffer::RenderBuffer(int width, int height, GLenum format)
	{
		glGenRenderbuffers(1, &_renderBufferHandle);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	RenderBuffer::~RenderBuffer()
	{
	}


	void RenderBuffer::bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferHandle);
	}


	void RenderBuffer::unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, NULL);
	}
}