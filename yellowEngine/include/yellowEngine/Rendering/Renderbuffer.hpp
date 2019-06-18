#ifndef __H_RENDERBUFFER__
#define __H_RENDERBUFFER__

#include <glad/glad.h>

namespace yellowEngine
{
	class Renderbuffer
	{
	public:
		Renderbuffer(int width, int height, GLenum format);
		~Renderbuffer();

		void bind();
		void unbind();

	private:
		unsigned int _renderbufferHandle;
	};
}

#endif