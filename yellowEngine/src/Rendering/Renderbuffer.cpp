#ifndef NULL
	#define NULL 0
#endif

#include "yellowEngine\Rendering\Renderbuffer.hpp"


namespace yellowEngine
{
	Renderbuffer::Renderbuffer(int width, int height, GLenum format)
	{
		glGenRenderbuffers(1, &_renderbufferHandle);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	Renderbuffer::~Renderbuffer()
	{
	}


	void Renderbuffer::bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferHandle);
	}


	void Renderbuffer::unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, NULL);
	}
}