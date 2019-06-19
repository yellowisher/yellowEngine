#ifndef NULL
	#define NULL 0
#endif

#include "yellowEngine\Rendering\RenderBuffer.hpp"


namespace yellowEngine
{
	RenderBuffer::RenderBuffer(GLenum internalFormat, int width, int height)
	{
		glGenRenderbuffers(1, &_renderBufferHandle);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
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